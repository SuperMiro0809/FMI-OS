#!/bin/bash

root_rss=$( ps -e -o user,rss --no-headers | awk '$1 == "root" { sum += $2 } END{ print sum}' )
ps -e -o user --no-headers | grep -v '^root' | sort -u | while read -r user; do
    home=$(grep "^$user:" /etc/passwd | awk -F: '{ print $6 }')
    if [ ! -d "$home" ] || [ "$(stat -c '%U' "$home")" != "$user" ] || [ "$(stat -c '%A' "$home" | cut -c3)" != "w" ]; then
        echo "$user"

        user_rss=$( ps -e -o user,rss --no-headers | awk -v user="$user" '$1 == user { sum += $2 } END{print sum}' )
        if [ $user_rss -gt $root_rss ]; then
            echo "Killing all processes of user: $user"
            pkill -u "$user"
        fi
    fi
done
