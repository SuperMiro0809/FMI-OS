#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <username>"
    exit 1
fi

user="$1"

pids=$(pgrep -u "$user")
count=$(echo $pids | wc -l)

if [ -z "$pids" ]; then
    echo "$user has no processes"
    exit 0
fi

kill $pids
echo "$count processes killed!"
