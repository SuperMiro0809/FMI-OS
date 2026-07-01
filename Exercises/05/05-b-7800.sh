#!/bin/bash

count=0

for dir in $(echo $PATH | tr ':' ' '); do
    count=$(( count + $( find "$dir" -maxdepth 1 -type f -executable 2>/dev/null | wc -l ) ))
done

echo "$count"
