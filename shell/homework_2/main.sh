#! /bin/bash

read n

a[0]=1
a[1]=1
for ((i = 2;i <= n;i ++))
do
    x=`expr $i - 1`
    y=`expr $i - 2`
    a[$i]=`expr ${a[$x]} + ${a[$y]}`
done

echo ${a[$n]}
