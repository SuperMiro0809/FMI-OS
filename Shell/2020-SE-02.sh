#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

file="$1"

top3=$(awk '{print $2}' "$file" | sort | uniq -c | sort -rn | head -3 | awk '{print $2}')

for host in $top3; do
    awk -v h="$host" '
    h == $2 {
        if ($8 == "HTTP/2.0") {
            count1[$2] += 1
        } else {
            count2[$2] += 1
        }
    }
    END {
        print h " HTTP/2.0: " count1[h]+0 " non-HTTP/2.0: " count2[h]+0
    }' "$file"

    awk -v h="$host" '
    h == $2 {
        if ($9 > 302) {
            clients[$1] += 1
        }
    }
    END{
        for (c in clients) {
            print clients[c], c
        }
    }' "$file" | sort -rn | head -5
done
