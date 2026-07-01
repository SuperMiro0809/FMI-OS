#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <zonefile1> [zonefile2...]"
    exit 1
fi

today=$(date '+%Y%m%d')

for file in "$@"; do
    serial=$(grep -oE '[0-9]{10}' "$file" | head -1)

    if [ -z "$serial" ]; then
        echo "Could not find serial in $file"
        continue
    fi

    serial_date="${serial:0:8}"
    serial_tt="${serial:8:2}"

    if [ "$serial_date" -lt "$today" ]; then
        new_serial="${today}00"
    elif [ "$serial_date" -eq "$today" ]; then
        tt=$((10#$serial_tt + 1))
        if [ $tt -gt 99 ]; then
            echo "Error: too many changes in $file today"
            continue
        fi
        new_serial=$(printf "%s%02d" "$today" $tt)
    else
        echo "Error: serial date is in the future in $file"
        continue
    fi

    sed -i "s/$serial/$new_serial/" "$file"
done
