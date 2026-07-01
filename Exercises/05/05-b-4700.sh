#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <num> [delimiter]"
    exit 1
fi

num="$1"
del="${2:- }"

echo "$num" | rev | fold -w3 | paste -sd "$del" | rev
