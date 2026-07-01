#!/bin/bash

show_hidden=false

if [ $1 = "-a" ]; then
    flag="-a"
    dir="$2"
else
    flag=""
    dir="$1"
fi

if [ -z "$dir" ]; then
    echo "Usage: $0 [-a] <directory>"
    exit 1
fi

ls $flag "$dir" | while read -r item; do
    full="$dir/$item"
    if [ -f "$full" ]; then
        echo "$item ($(wc -c < "$full") bytes)"
    elif [ -d "$full" ]; then
        echo "$item ($(ls $flag "$full" | wc -l) entries)"
    fi
done
