#!/bin/bash

if [ "$(whoami)" != "oracle" ] && [ "$(whoami)" != "grid" ]; then
    echo "Must be run as oracle or grid"
    exit 1
fi

if [ -z "$ORACLE_HOME" ]; then
    echo "ORACLE_HOME not set"
    exit 1
fi

adrci="$ORACLE_HOME/bin/adrci"

if [ ! -x "$adrci" ]; then
    echo "$adrci not found or not executable"
    exit 1
fi

diag_dest="/u01/app/$(whoami)"

output=$("$adrci" exec="show homes")

if echo "$output" | grep -q "No ADR homes are set"; then
    echo "No ADR homes found"
    exit 0
fi

echo "$output" | grep -v "ADR Homes:" | while read -r home; do
    [ -z "$home" ] && continue
    full_path="$diag_dest/$home"
    size=$(du -sm "$full_path" 2>/dev/null | cut -f1)
    echo "${size:-0} $full_path"
done
