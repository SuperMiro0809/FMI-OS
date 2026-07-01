#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 [-r|-g|-b] <string"
    exit 1
fi

if [ $# -eq 1 ]; then
    echo "$1"
    exit 0
fi

color="$1"
text="$2"

case "$color" in
    -r) echo -e "\033[0;31m$text" ;;
    -g) echo -e "\033[0;32m$text" ;;
    -b) echo -e "\033[0;34m$text" ;;
esac

echo -e '\033[0m'
