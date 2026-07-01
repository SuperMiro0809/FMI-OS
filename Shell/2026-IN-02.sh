#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <file> <k>"
    exit 1
fi

file="$1"
k="$2"

from="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
to=$(echo "$from" | cut -c$((k+1))-26)$(echo "$from" | cut -c1-$k)

grep -oE '[A-Z]+' "$file" | tr "$from" "$to" | sort -u | tr 'A-Z' 'a-z' | grep -Fxf /usr/share/dict/words | wc -l
