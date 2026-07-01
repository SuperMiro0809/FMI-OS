#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <shared_dir>"
    exit 1
fi

dir="$1"

if [ ! -d "$1" ]; then
    echo "$dir must be a directory"
    exit 1
fi

group=$(stat -c '%G' "$dir")

if [ "$(id -u)" -eq 0 ]; then
    find "$dir" | while read -r item; do
        if [ -d "$item" ]; then
            chmod o-rwx "$item"
            chmod g+rwx "$item"
            chown :"$group" "$item"
        elif [ -f "$item" ]; then
            chmod o-rwx "$item"
            chmod g+rw "$item"
            chown :"$group" "$item"
        fi
    done
elif id -Gn | grep -qw "$group"; then
    umask 007
else
    echo "You are not a member of group $group"
    exit 1
fi
