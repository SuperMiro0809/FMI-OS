#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <config_file>"
    exit 1
fi

config_file="$1"

cat "$config_file" | while read -r dir type mask; do
    case "$type" in
        R) perm_flag="$mask" ;;
        A) perm_flag="/$mask" ;;
        T) perm_flag="-$mask" ;;
    esac

    find "$dir" -mindepth 1 \( -type f -o -type d \) -perm $perm_flag | while read -r item; do
        if [ -d "$item" ]; then
            chmod 755 "$item"
        elif [ -f "$item" ]; then
            chmod 664 "$item"
        fi
    done
done
