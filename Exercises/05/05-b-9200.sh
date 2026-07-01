#!/bin/bash

recursive=false

if [ "$1" = "-r" ]; then
    recursive=true
    shift
fi

if [ -z "$RMLOG_FILE" ]; then
    echo "RMLOG_FILE not set"
    exit 1
fi

timestamp() {
    date '+%Y-%m-%d %H:%M:%S'
}

for item in "$@"; do
    if [ -f "$item" ]; then
        rm "$item"
        echo "[$(timestamp)] Removed file $item" >> "$RMLOG_FILE"
    elif [ -d "$item" ]; then
        if [ -z "$(ls "$item")" ]; then
            rmdir "$item"
            echo "[$(timestamp)] Removed directory $item" >> "$RMLOG_FILE"
        elif [ "$recursive" = true ]; then
            rm -r "$item"
            echo "[$(timestamp)] Removed directory recursively $item" >> "$RMLOG_FILE"
        else
            echo "$item is not empty, skipping"
        fi
    fi
done
