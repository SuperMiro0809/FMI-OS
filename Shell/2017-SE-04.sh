#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
    echo "Usage: $0 <dir> [file]"
    exit 1
fi

dir="$1"
file="$2"

output() {
    if [ -z "$file" ]; then
        echo "$1"
    else
        echo "$1" >> "$file"
    fi
}

broken=0
while IFS= read -r link; do
    if [ ! -e "$link" ]; then
        broken=$(( broken + 1 ))
    else
        output "$(basename "$link") -> "$link""
    fi
done < <(find "$dir" -type l)

output "Broken symlinks: $broken"
