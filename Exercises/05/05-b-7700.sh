#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <dir> <num>"
    exit 1
fi

dir="$1"
num="$2"

ls -l "$dir" | awk -v sz=$num '$5 > sz { sum += $5 } END{ print sum }'

# find "$dir" -maxdepth 1 -type f -size +${num}c | xargs wc -c | tail -1 | awk '{print $1}'
