#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <old_dir> <new_dir>"
    exit 1
fi

olddir="$1"
newdir="$2"

while IFS= read -r file; do
    declare local -A keys

    while IFS== read -r key val; do
        echo $key, $val
    done < <(cat "$file")

done < <(find "$olddir" -type f -name '*.bcf')
