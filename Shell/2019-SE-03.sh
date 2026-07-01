#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <dir>"
    exit 1
fi

dir="$1"
hashfile="/tmp/.last_hashes"

extracted=~/extracted
mkdir -p "$extracted"

find "$dir" -type f -name "*_report-*.tgz" | while read -r archive; do
    filename=$(basename "$archive")
    name=$(echo "$filename" | sed 's/_.*//')
    timestamp=$(echo "$filename" | sed 's/.*report-//' | sed 's/\.tgz//')

    current_hash=$(sha256sum "$archive" | cut -c1-64)
    last_hash=$(grep "$filename" "$hashfile" 2>/dev/null | cut -c1-64)

    if [ "$current_hash" != "$last_hash" ]; then
        grep -v "$filename" "$hashfile" > /tmp/.tmp_hashes 2>/dev/null
        echo "$current_hash  $filename" >> /tmp/.tmp_hashes
        mv /tmp/.tmp_hashes "$hashfile"

        if tar -tzf "$archive" | grep -q "^meow.txt$"; then
            tar -xzf "$archive" -C /tmp meow.txt
            mv /tmp/meow.txt "$extracted/${name}_${timestamp}.txt"
        fi
    fi
done
