#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 <src> <dst> <str>"
    exit 1
fi

src="${1%/}"
dst="$2"
str="$3"

if [ ! -d "$src" ]; then
    echo "SRC is not a directory"
    exit 1
fi

if [ ! -d "$dst" ] || [ -n "$( ls "$dst" )" ]; then
    echo "$dst must be an empty directory"
    exit 1
fi

find "$src" -type f -name "*$str*" | while read -r file; do
    rel="$( echo "$file" | sed "s|^$src/||" )"
    dest_dir="$dst/$(dirname "$rel")"
    mkdir -p "$dest_dir"
    mv "$file" "$dest_dir/"
done
