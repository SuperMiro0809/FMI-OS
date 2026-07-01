#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <csv1> <csv2>"
    exit 1
fi

csv1="$1"
csv2="$2"

cat "$csv1" | sort -t, -k2 | awk -F, '
{
    key=""
    for (i=2;i<=NF;i++) key = key","$i

    if (!(key in seen)) {
        seen[key] = $0
        ids[key] = $1
    } else if ($1 < ids[key]) {
        seen[key] = $0
        ids[key] = $1
    }
}
END{
    for (k in seen) {
        print seen[k]
    }
}' | sort -t, -k1 -n > "$csv2"
