#!/bin/bash

if [ -z "$BACKUP_DIR" ]; then
    echo "BACKUP_DIR not set"
    exit 1
fi

format_timestamp() {
    echo "$1" | sed 's/\(....\)-\(..\)-\(..\)-\(..\)-\(..\)-\(..\)/\1\/\2\/\3 \4:\5:\6/'
}

if [ "$1" = "-l" ]; then
    for f in "$BACKUP_DIR"/*; do
        name=$(basename "$f")
        filename=$(echo "$name" | sed 's/_[0-9-]*\.\(gz\|tgz\)$//')
        ts=$(echo "$name" | grep -oE '[0-9]{4}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}')
        ts_fmt=$(format_timestamp "$ts")
        echo "$filename  ($ts_fmt)"
    done
    exit 0
fi

if [ $# -lt 1 ]; then
    echo "Usage: $0 [-l] <filename> [dest_dir]"
    exit 1
fi

name="$1"
dest="${2:-.}"

matches=$(ls "$BACKUP_DIR" | grep "^${name}_")

count=$(echo "$matches" | wc -l)

if [ -z "$matches" ]; then
    echo "No backup found for $name"
    exit 1
fi

if [ "$count" -gt 1 ]; then
    i=1
    echo "$matches" | while read -r f; do
        ts=$(echo "$f" | grep -oE '[0-9]{4}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}')
        ts_fmt=$(format_timestamp "$ts")
        echo "($i) $name  ($ts_fmt)"
        i=$((i+1))
    done
    read -p "choose file (1-$count): " choice
    selected=$(echo "$matches" | sed -n "${choice}p")
else
    selected="$matches"
fi

filepath="$BACKUP_DIR/$selected"

if echo "$selected" | grep -q '\.tgz$'; then
    tar -xzf "$filepath" -C "$dest"
else
    gunzip -c "$filepath" > "$dest/$name"
fi

rm "$filepath"
