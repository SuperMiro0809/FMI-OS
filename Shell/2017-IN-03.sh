#!/bin/bash

newest_file=""
newest_user=""

while IFS=: read -r username _ _ _ _ homedir _; do
    if [ ! -d "$homedir" ]; then
        continue
    fi

    file=$( find "$homedir" -maxdepth 1 -type f -printf '%T@ %p\n' 2>/dev/null | sort -rn | head -1 | awk '{print $2}' )

    if [ -z "$file" ]; then
        continue
    fi

    if [ -z "$newest_file" ] || [ "$file" -nt "$newest_file" ]; then
        newest_file="$file"
        newest_user="$username"
    fi
done < /etc/passwd

if [ -z "$newest_file" ]; then
    echo "No newest file"
else
    echo "User: $newest_user"
    echo "File: $newest_file"
fi
