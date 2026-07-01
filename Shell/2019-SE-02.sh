#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 [-n N] FILE1..."
    exit 1
fi

num=10
if [ "$1" = '-n' ]; then
    num="$2"
    shift 2
fi

for file in "$@"; do
    idf=$(basename "$file" .log)
    tail -n "$num" "$file" | awk -v idf="$idf" '{
        print $1, $2, idf, substr($0, 20)
    }'
done | sort
