#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <bin_file> <output_file>"
    exit 1
fi

input="$1"
output="$2"

size=$(wc -c < "$input")
max=$((524288 * 2))

if [ "$size" -gt "$max" ]; then
    echo "Input file too large"
    exit 1
fi

count=$((size / 2))

elements=$(od -v -An -t x2 -w2 "$input" | tr -d ' \n' | sed 's/.\{4\}/0x&, /g' | sed 's/, $//')

cat > "$output" << EOF
#ifndef ARR_H
#define ARR_H

#include <stdint.h>

const uint16_t arr[] = { $elements };
const uint32_t arrN = $count;

#endif
EOF
