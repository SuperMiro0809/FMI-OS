#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 username"
    exit 1
fi

user="$1"

while true; do
    if w | grep -q "^$user"; then
        echo "$user is logged in"
        exit 0
    fi
    sleep 1
done
