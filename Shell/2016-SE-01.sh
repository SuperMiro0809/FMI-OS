#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

dir="$1"

if [ ! -d "$dir" ]; then
    echo "$dir is not a directory"
    exit 1
fi

find "$dir" -type l | while read -r link; do
    if [ ! -e "$link" ]; then
        echo "$link"
    fi
done
