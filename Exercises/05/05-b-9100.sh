#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <src> <dest>"
    exit 1
fi

src="$1"
dest="$2"

find "$src" -type f | sed '\.' | sed 's/^.*\.//' | sort -u | while read -r type; do
    mkdir "$dest/$type"
done

find "$src" -type f | sed '\.' | while read -r file; do
    type=$( echo "$file" | sed 's/^.*\.//' )
    mv "$file" "$dest/$type"
done
