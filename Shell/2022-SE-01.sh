#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <config_file>"
    exit 1
fi

config_file="$1"

cat "$config_file" | sed -E 's/ *#.*//' | while read -r line; do
    if [ -z "$line" ]; then
        continue
    fi

    device=$(echo "$line" | awk '{print $1}' | sed 's/^ *//;s/ *$//')
    operation=$(echo "$line" | awk '{print $2}' | sed 's/^ *//;s/ *$//')

    curr_d=$(grep "$device" /proc/acpi/wakeup)

    if [ -z "$curr_d" ]; then
        echo "Device $device not found"
        continue
    fi

    curr_o=$(echo "$curr_d" | awk '{print $3}')

    if [ "$operation" == "${curr_o#\*}" ]; then
        echo "Device $device is already $operation"
        continue
    else 
        echo "$device" > /proc/acpi/wakeup
    fi
done
