#!/bin/bash

for el in "$@"; do
    if [ -f "$el" ] && [ -r "$el" ]; then
        echo "$el is readable file"
    elif [ -d "$el" ]; then
        count=$( ls "$el" | wc -l )

        echo $( ls -l "$el" | awk -v cnt="$count" ' $5 < cnt { print $9 } ' )
    fi
done
