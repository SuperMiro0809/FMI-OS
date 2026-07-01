#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <source_dir> [dest_dir]"
    exit 1
fi

src="$1"

if [ $# -eq 2 ]; then
    dest="$2"
else 
    dest=$(date +%Y-%m-%d)
fi

mkdir -p "$dest"

find "$src" -maxdepth 1 -type f -mmin -45 -exec cp {} "$dest" \;

echo "Copied in $dest"
read -p "Do you want to arhive? (y/n): " choice

if [ "$choice" = "y" ]; then
    tar -czf "$dest.tar.qz" "$dest"
    echo "Archived $dest.tar.qz"
fi
