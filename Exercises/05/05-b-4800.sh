#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <file> <dir>"
    exit 1
fi

file="$1"
dir="$2"

find "$dir" -type f | while read -r f; do
    if cmp -s "$file" "$f"; then
        echo "$f"
    fi
done
