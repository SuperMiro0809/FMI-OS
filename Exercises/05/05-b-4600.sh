#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 <num> <left> <right>"
    exit 4
fi

is_integer() {
    echo "$1" | grep -qE '^-?[0-9]+$'
}

if ! is_integer "$1" || ! is_integer "$2" || ! is_integer "$3"; then
    exit 3
fi

if [ $2 -gt $3 ]; then
    exit 2
fi

if [ $1 -lt $2 ] || [ $1 -gt $3 ]; then
    exit 1
fi

exit 0
