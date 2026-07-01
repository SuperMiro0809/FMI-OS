#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <config_file> <input_file>"
    exit 1
fi

config_file="$1"
input_file="$2"

name=$(basename "$input_file" | sed 's/\..*//')

cat "$config_file" | while read -r line; do
    lang=$(echo "$line" | awk '{print $1}')
    dir=$(echo "$line" | awk '{print $NF}' | sed "s/'//g")

    recognizer=$(echo "$line" | grep -c 'recognizer')
    listener=$(echo "$line" | grep -c 'listener')
    visitor=$(echo "$line" | grep -c 'visitor')

    comm="antlr4 -Dlanguage=$lang "

    if [ $listener -eq 0 ]; then
        comm="${comm}-no-listener "
    fi

    if [ $visitor -eq 1 ]; then
        comm="${comm}-visitor "
    fi
    
    comm="${comm}-o $dir/$name $input_file"
    echo $comm
done
