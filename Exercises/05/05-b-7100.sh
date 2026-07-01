#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <dir_name> <num>"
    exit 1
fi

dir="$1"
num="$2"

find "$dir" -type f -size +${num}c 
