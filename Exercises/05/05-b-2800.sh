#!/bin/bash

if [ $# -ne 1 ]; then
    echo "One parameter is required"
    exit 1
fi

if echo "$1" | grep -qE '^[a-zA-Z0-9]+$'; then
    echo "Contains only letters and number"
else
    echo "Contains special characters"
fi
