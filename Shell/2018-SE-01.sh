#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <dir>"
    exit 1
fi

dir="$1"
declare -A friends

while read -r file;do
    friend=$(basename "$(dirname "$file")")

    friends["$friend"]=$(( friends["$friend"] + $(wc -l < "$file") ))
done < <(find "$dir" -regextype posix-extended  -type f -regex '.*/[0-9]{4}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}\.txt')

for f in "${!friends[@]}"; do
    echo "$f ${friends[$f]}"
done | sort -k2 -rn | head -10
