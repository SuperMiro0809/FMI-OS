#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 <file> <key1> <key2>"
    exit 1
fi

file="$1"
key1="$2"
key2="$3"

val1=$( grep "^$key1=" "$file" | cut -d= -f2 )
val2=$( grep "^$key2=" "$file" | cut -d= -f2 )

unique=$( echo "$val2" | tr ' ' '\n' | grep -vF "$( echo "$val1" | tr ' ' '\n' )" | tr '\n' ' ' )

sed -i "s/^$key2=.*/$key2=$unique/" "$file"
