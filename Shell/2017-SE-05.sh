#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <dir> <str>"
    exit 1
fi

dir="$1"
str="$2"

find "$dir" -maxdepth 1 -type f | xargs -I{} basename {} | grep -E "^vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-$str" | \
    sort -t'-' -k2 -V -r | head -1
