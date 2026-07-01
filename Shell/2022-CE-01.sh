#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 <num> <prefix_symbol> <unit_symbol>"
    exit 1
fi

num="$1"
prefix_symbol="$2"
unit_symbol="$3"

decimal=$(awk -F, -v sym="$prefix_symbol" '$2 == sym { print $3 }' 'prefix.csv')
unit_name=$(awk -F, -v sym="$unit_symbol" '$2 == sym { print $1 }' 'base.csv')
measure=$(awk -F, -v sym="$unit_symbol" '$2 == sym { print $3 }' 'base.csv')

echo "$(echo "$num * $decimal" | bc) $unit_symbol ($measure, $unit_name)"
