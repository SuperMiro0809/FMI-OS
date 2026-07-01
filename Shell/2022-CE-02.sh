#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <device>"
    exit 1
fi

name="$1"
status=$(awk -v name="$name" '$1 == name {print $3}' /proc/acpi/wakeup)

if [ -z "$status" ]; then
    echo "Device $name not found"
    exit 1
fi

if [ "$status" == "*enabled" ]; then
    echo "$name" > /proc/acpi/wakeup
fi
