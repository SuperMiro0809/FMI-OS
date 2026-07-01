#!/bin/bash

while IFS=: read -r username _ _ _ _ homedir _; do
    if [ ! -d "$homedir" ] || [ ! -w "$homedir" ]; then
        echo "$username: $homedir"
    fi
done < /etc/passwd
