#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <dir>"
    exit 1
fi

dir="$1"

if [ ! -d "$dir" ]; then
    echo "$dir must be a directory"
    exit 1
fi

data="$dir/.data"
mkdir -p "$data"


find "$dir" -path "$data" -prune -o -type f -print | while read -r file; do
    hash=$(sha256sum "$file" | cut -c1-64)
    
    if [ ! -f "$data/$hash" ]; then
        cp "$file" "$data/$hash"
        rm "$file"
        ln -s "$(realpath --relative-to="$(dirname "$file")" "$data/$hash")" "$file"
    fi

done
