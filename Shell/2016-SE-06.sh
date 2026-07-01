#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

file="$1"

awk '{ print NR ". " $0 }' "$file" | sed -E 's/^([0-9]+\. )[0-9]+ г. - /\1/' | sort -k2
