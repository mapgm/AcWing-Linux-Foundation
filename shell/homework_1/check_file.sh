#! /bin/bash

if [ $# -ne 1 ]
then
    echo arguments not valid
    exit 1
fi

if ! [ -e "$1" ]
then
    echo not exist
    exit 2
fi

if [ -f "$1" ]
then
    echo regular file
fi

if [ -d "$1" ]
then
    echo directory
fi

if [ -r "$1" ]
then
    echo readable
fi

if [ -w "$1" ]
then
    echo writable
fi

if [ -x "$1" ]
then
    echo executable
fi
