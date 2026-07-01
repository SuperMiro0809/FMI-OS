#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
    echo "Usage: $0 <dir> [num]"
    exit 1
fi

dir="$1"

if [ ! -z "$2" ]; then
    num="$2"
fi

if [ -z "$num" ]; then
    find "$dir" -type l | while read -r link; do
        if [ ! -e "$link" ]; then
            echo "$link"
        fi
    done
else
 find "$dir" -type f -links +"$(( num - 1 ))"
fi
