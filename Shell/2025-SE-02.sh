#!/bin/bash

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
    echo "Usage: $0 <operation> [service_name]"
    exit 1
fi

operation="$1"

if [ "$operation" != "start" ] && [ "$operation" != "stop" ] && [ "$operation" != "running" ] && [ "$operation" != "cleanup" ]; then
    echo "Allowed operations: start, stop, cleanup"
    exit 1
fi

if [ -z "$SVC_DIR" ]; then
    echo "SVC_DIR is required"
    exit 1
fi

if [ "$operation" == "start" ] || [ "$operation" == "stop" ]; then
    if [ $# -ne 2 ]; then
        echo "Usage: $0 start|stop <service>"
        exit 1
    fi
    service="$2"
fi

getvalue() {
    cat "$1" | grep -E "^$2:" | awk -F: '{print $2}' | sed 's/^ *//;s/ *$//' 
}

find "$SVC_DIR" -type f | while read -r file; do
    name=$(getvalue "$file" 'name')
    pidfile=$(getvalue "$file" 'pidfile')
    outfile=$(getvalue "$file" 'outfile')
    comm=$(grep -E '^comm:' "$file" | sed 's/^comm: *//')

    if [ "$operation" == "start" ] && [ "$service" == "$name" ]; then
        if [ -f "$pidfile" ] && kill -0 $(cat "$pidfile") 2>/dev/null; then
            echo "$name already running"
        else
            bash -c "$comm" >> "$outfile" 2>&1 &
            echo $! > "$pidfile"
        fi
    elif [ "$operation" = "stop" ] && [ "$service" = "$name" ]; then
        kill -TERM $(cat "$pidfile")
    elif [ "$operation" = "running" ]; then
        if [ -f "$pidfile" ] && kill -0 $(cat "$pidfile") 2>/dev/null; then
            echo "$name"
        fi
    elif [ "$operation" = "cleanup" ]; then
        if [ -f "$pidfile" ] && ! kill -0 $(cat "$pidfile") 2>/dev/null; then
            rm "$pidfile"
        fi
    fi
done | sort
