#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <file1> <file2>"
    exit 1
fi

file1="$1"
file2="$2"

count1=$( grep -c "$(basename "$file1")" "$file1" )
count2=$( grep -c "$(basename "$file2")" "$file2" )

if [ $count1 -gt $count2 ]; then
    winner=$file1
else
    winner=$file2
fi

name=$( basename "$winner" )
sed -E 's/^[^ ]* [^ ]* - //' "$winner" | sort > "$name.songs"
