#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <dir>"
    exit 1
fi

dir="$1"

find "$dir" -maxdepth 1 -type f | sort | while read -r file; do
    if [ -f "$file" ]; then
        find "$dir" -maxdepth 1 -type f | sort | while read -r other; do
            if [ "$file" != "$other" ] && cmp -s "$file" "$other"; then
                rm "$other"
                echo "$other is deleted"
            fi
        done
    fi
done
