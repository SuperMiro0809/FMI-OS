#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <file> <dir>"
    exit 1
fi

file="$1"
dir="$2"

if [ ! -z "$( ls -A "$dir" )" ]; then
    echo "Dir must be empty"
    exit 1
fi

touch "$dir/dict.txt"

declare -A people
number=1

while IFS=: read -r first second; do
    name=$( echo "$first" | sed -E 's/ *\(.*\)//' )
    second=$( echo "$second" | sed -E 's/^ *//')

    if [ -z "${people[$name]}" ]; then
        people[$name]=$number
        echo "$name;$number" >> "$dir/dict.txt"

        touch "$dir/$number.txt"
        echo "$second" >> "$dir/$number.txt"

        number=$(( number + 1 ))
    else
        echo "$second" >> "$dir/${people[$name]}.txt"
    fi
done < "$file"
