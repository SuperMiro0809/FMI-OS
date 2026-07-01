#!/bin/bash

case "$1" in
    -r) colors=("\033[0;31m" "\033[0;32m" "\033[0;34m") ;;
    -g) colors=("\033[0;32m" "\033[0;34m" "\033[0;31m") ;;
    -b) colors=("\033[0;34m" "\033[0;31m" "\033[0;32m") ;;
    -x) cat; exit 0 ;;
    *) echo "Usage: $0 [-r|-g|-b|-x]"; exit 1 ;;
esac

i=0
while IFS= read -r line; do
    echo -e "${colors[$i]}$line"
    i=$(( (i + 1) % 3 ))
done

echo -e '\033[0m'
