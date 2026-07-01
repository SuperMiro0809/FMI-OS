#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Please, provide file!"
    exit 1
fi

max=0
current=0

for line in $(cat "$1"); do
    opens=$( echo "$line" | tr -cd '{' | wc -c )
    closes=$( echo "$line" | tr -cd '}' | wc -c )

    current=$(( $current + $opens - $closes ))
    if [ $current -gt $max ]; then
        max=$current
    fi
done

echo "File deepest nesting is $max levels"
