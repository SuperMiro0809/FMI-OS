#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <num1> <num2>"
    exit 1
fi

num1="$1"
num2="$2"

if [ "$num1" -gt "$num2" ]; then
    echo "Num1 should be less than num2"
    exit 1
fi

mkdir -p a b c

find -type f | while read -r file; do
    rows=$(wc -l < "$file")

    if [ $rows -lt $num1 ]; then
        mv "$file" "./a"
    elif [ $rows -gt $num1 ] && [ $rows -lt $num2 ]; then
        mv "$file" "./b"
    else
        mv "$file" "./c"
    fi
done
