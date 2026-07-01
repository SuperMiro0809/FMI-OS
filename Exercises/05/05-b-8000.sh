#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <user>"
    exit 1
fi

ps -u "$1" -o pid,rss,vsz --no-headers | sort -k3 -rn | while read -r pid rss vsz; do
    ratio=$( echo "scale=2; $rss / $vsz" | bc )
    echo "PID $pid: RSZ/VSZ = $ratio"
done
