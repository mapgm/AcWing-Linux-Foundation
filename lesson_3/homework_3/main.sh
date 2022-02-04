#! /bin/bash

read n
read m

for ((i = 1;i <= n;i ++))
do
    st[$i]=0
done

dfs()
{
    if [ $1 -gt $n ]
    then
        m=`expr $m - 1`
        if [ $m -eq 0 ]
        then
            for((i = 1;i <= n;i ++))
            do
                echo -e "${path[$i]} \c"
            done
            echo -e "\n"
            return 0
        fi
        return 1
    fi

    local j=0
    for((j = 1;j <= n;j ++))
    do
        if [ ${st[$j]} -eq 0 ]
        then
            path[$1]=$j
            st[$j]=1
            if dfs `expr $1 + 1`
            then
                return 0
            fi
            st[$j]=0
        fi
    done

    return 1
}

dfs 1
