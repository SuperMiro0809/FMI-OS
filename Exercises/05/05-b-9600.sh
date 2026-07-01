#!/bin/bash

recursive=false

if [ "$1" = "-r" ]; then
    recursive=true
    shift
fi

if [ -z "$BACKUP_DIR" ]; then
    echo "BACKUP_DIR not set"
    exit 1
fi

mkdir -p "$BACKUP_DIR"

timestamp() {
    date '+%Y-%m-%d-%H-%M-%S'
}

for item in "$@"; do
    name=$(basename "$item")
    ts=$( timestamp )

    if [ -f "$item" ]; then
        gzip -c "$item" > "$BACKUP_DIR/${name}_${ts}.gz"
        rm "$item"
    elif [ -d "$item" ]; then
        if [ -z "$(ls "$item")" ]; then
            tar -czf "$BACKUP_DIR/${name}_${ts}.tgz" "$item"
            rmdir "$item"

        elif [ "$recursice" = true ]; then
             tar -czf "$BACKUP_DIR/${name}_${ts}.tgz" "$item"
             rm -r "$item"

        else
            echo "error: $item is not empty, will not delete"
        fi
    fi
done
