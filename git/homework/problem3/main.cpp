#include <iostream>

using namespace std;

const int N = 1010;

int n, m;
int f[N];

int main()
{
    cin >> n >> m;

    while (n -- )
    {
        int v, w;
        cin >> v >> w;
        for (int j = v; j <= m; j ++ )
            f[j] = max(f[j], f[j - v] + w);
    }

    cout << f[m] << endl;
    return 0;
}
