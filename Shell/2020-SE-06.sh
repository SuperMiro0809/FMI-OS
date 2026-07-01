#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 <conf_file> <key> <value>"
    exit 1
fi

conffile="$1"
key="$2"
value="$3"

content=""
found=0
while IFS= read -r line; do
    comm=$(echo "$line" | grep -E '^\s*#')

    if [ -z "$comm" ]; then
        row=$(echo "$line" | sed 's/ *#.*//' | sed 's/^ *//;s/ *$//')
        k=$(echo "$row" | cut -d= -f1 | sed 's/^ *//;s/ *$//')
        v=$(echo "$row" | cut -d= -f2 | sed 's/^ *//;s/ *$//')

        if [ "$k" == "$key" ]; then
            found=1
            if [ "$v" != "$value" ]; then
                content="${content}# $line # edited at $(date) by $(whoami)\n"
                content="${content}$key = $value # added at $(date) by $(whoami)\n"
            fi
        else 
            content="${content}$line\n"
        fi
    else
        content="${content}$line\n"
    fi
done < "$conffile"

if [ "$found" -eq 0 ]; then
    content="${content}$key = $value # added at $(date) by $(whoami)\n"
fi

printf "%b" "$content" > "$conffile"
