#! /bin/bash


#**********  lesson_1/homework_0  **********

dir0=/home/acs/homework/lesson_1/homework_0

homework 1 create 0

for i in dir_a dir_b dir_c
do
    mkdir ${dir0}/${i}
done

#**********  lesson_1/homework_1  **********

dir1=/home/acs/homework/lesson_1/homework_1

homework 1 create 1

for i in a.txt b.txt c.txt
do
    cp ${dir1}/${i} ${dir1}/${i}.bak
done

#**********  lesson_1/homework_2  **********

dir2=/home/acs/homework/lesson_1/homework_2

homework 1 create 2

for i in a b c
do
    mv ${dir2}/${i}.txt ${dir2}/${i}_new.txt
done

#**********  lesson_1/homework_3  **********

dir3=/home/acs/homework/lesson_1/homework_3

homework 1 create 3

# mv ${dir3}/dir_a/* ${dir3}/dir_b

for i in a.txt b.txt c.txt
do
    mv ${dir3}/dir_a/${i} ${dir3}/dir_b
done

#**********  lesson_1/homework_4  **********

dir4=/home/acs/homework/lesson_1/homework_4

homework 1 create 4

for i in a.txt b.txt c.txt
do
    rm ${dir4}/${i}
done

#**********  lesson_1/homework_5  **********

dir5=/home/acs/homework/lesson_1/homework_5

homework 1 create 5

for i in dir_a dir_b dir_c
do
    rm ${dir5}/${i} -r
done

#**********  lesson_1/homework_6  **********

dir6=/home/acs/homework/lesson_1/homework_6

homework 1 create 6

mv ${dir6}/task.txt "${dir6}/done.txt"
mkdir ${dir6}/dir_a
mv "${dir6}/done.txt" ${dir6}/dir_a

#**********  lesson_1/homework_7  **********

dir7=/home/acs/homework/lesson_1/homework_7

homework 1 create 7

for((i = 0;i <= 2;i ++))
do
    mkdir ${dir7}/dir_${i}
    for j in a b c
    do
        cp ${dir7}/${j}.txt ${dir7}/dir_${i}/${j}${i}.txt
    done
done

#**********  lesson_1/homework_8  **********

dir8=/home/acs/homework/lesson_1/homework_8

homework 1 create 8

rm ${dir8}/dir_a/a.txt
mv ${dir8}/dir_b/b.txt ${dir8}/dir_b/b_new.txt
cp ${dir8}/dir_c/c.txt ${dir8}/dir_c/c.txt.bak

#**********  lesson_1/homework_9  **********

dir9=/home/acs/homework/lesson_1/homework_9

homework 1 create 9

rm ${dir9}/*.txt



# homework 1 test
