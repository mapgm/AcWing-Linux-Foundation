#include "match_server/Match.h"
#include "save_client/Save.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/transport/TSocket.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/TToString.h>
#include <thrift/server/TThreadedServer.h>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <unistd.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace ::match_service;
using namespace ::save_service;
using namespace std;

struct Task
{
    User user;
    string type;
};

struct MessageQueue
{
    queue<Task> q;
    mutex m;
    condition_variable cv;
}message_queue;

class Pool
{
    public:
        void save_result(int a, int b)
        {
            printf("Match Result: %d %d\n", a, b);

            std::shared_ptr<TTransport> socket(new TSocket("123.57.47.211", 9090));
            std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
            std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
            SaveClient client(protocol);

            try {
                transport->open();

                int res = client.save_data("acs_3247", "f8745d5e", a, b);

                if (!res) cout << "Successfully uploaded to the server!" << endl;
                else cout << "Failed to upload to the server!" << endl;

                transport->close();
            } catch (TException& tx) {
                cout << "ERROR: " << tx.what() << endl;
            }
        }

        bool check_match(uint32_t i, uint32_t j)
        {
            auto a = users[i], b = users[j];

            int dt = abs(a.score - b.score);
            int a_max_dif = waiting_time[i] * 50;
            int b_max_dif = waiting_time[j] * 50;

            return dt <= a_max_dif && dt <= b_max_dif;
        }

        void match()
        {
            for (uint32_t i = 0;i < waiting_time.size();i ++)
                waiting_time[i] ++; // 等待时间 + 1

            while (users.size() > 1)
            {
                bool flag = true;
                for (uint32_t i = 0;i < users.size();i ++)
                {
                    for (uint32_t j = i + 1;j < users.size();j ++)
                    {
                        if (check_match(i, j))
                        {
                            auto a = users[i], b = users[j];
                            users.erase(users.begin() + j);
                            users.erase(users.begin() + i);
                            waiting_time.erase(waiting_time.begin() + j);
                            waiting_time.erase(waiting_time.begin() + i);
                            save_result(a.id, b.id);
                            flag = false;
                            break;
                        }
                    }

                    if (!flag) break;
                }

                if (flag) break;
            }
        }

        void add(User user)
        {
            users.push_back(user);
            waiting_time.push_back(0);
        }

        void remove(User user)
        {
            for (uint32_t i = 0;i < users.size();i ++)
            {
                if (users[i].id == user.id)
                {
                    users.erase(users.begin() + i);
                    waiting_time.erase(waiting_time.begin() + i);
                    break;
                }
            }
        }
    private:
        vector<User> users;
        vector<int> waiting_time;

}pool;

class MatchHandler : virtual public MatchIf {
    public:
        MatchHandler() {
            // Your initialization goes here
        }

        int32_t add_user(const User& user, const std::string& info) {
            // Your implementation goes here
            printf("add_user\n");

            unique_lock<mutex> lck(message_queue.m);
            message_queue.q.push({user, "add"});
            message_queue.cv.notify_all();

            return 0;
        }

        int32_t remove_user(const User& user, const std::string& info) {
            // Your implementation goes here
            printf("remove_user\n");

            unique_lock<mutex> lck(message_queue.m);
            message_queue.q.push({user, "remove"});
            message_queue.cv.notify_all();

            return 0;
        }

};

class MatchCloneFactory : virtual public MatchIfFactory {
    public:
        ~MatchCloneFactory() override = default;
        MatchIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) override
        {
            std::shared_ptr<TSocket> sock = std::dynamic_pointer_cast<TSocket>(connInfo.transport);
            /*
            cout << "Incoming connection\n";
            cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
            cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
            cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
            cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
            */
            return new MatchHandler;
        }
        void releaseHandler(MatchIf* handler) override {
            delete handler;
        }
};

void comsume_task()
{
    while(true)
    {
        unique_lock<mutex> lck(message_queue.m);
        if (message_queue.q.empty())
        {
            // message_queue.cv.wait(lck);
            lck.unlock();
            pool.match();
            sleep(1);
        }
        else
        {
            auto task = message_queue.q.front();
            message_queue.q.pop();
            lck.unlock();

            if (task.type == "add") pool.add(task.user);
            else if (task.type == "remove") pool.remove(task.user);
        }
    }
}

int main(int argc, char **argv) {
    TThreadedServer server(
            std::make_shared<MatchProcessorFactory>(std::make_shared<MatchCloneFactory>()),
            std::make_shared<TServerSocket>(9090), //port
            std::make_shared<TBufferedTransportFactory>(),
            std::make_shared<TBinaryProtocolFactory>());


    cout << "Start Match Server!" << endl;

    thread matching_thread(comsume_task);

    server.serve();
    return 0;
}
