#!/bin/bash

read -p "Enter string: " str

for file in "$@"; do
    count=$(grep "$str" "$file" | wc -l)
    echo "$file: $count"
done
