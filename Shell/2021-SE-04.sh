#!/bin/bash

if [ "$(whoami)" != "oracle" ] && [ "$(whoami)" != "grid" ]; then
    echo "Must be run as oracle or grid"
    exit 1
fi

if [ $# -ne 1 ]; then
    echo "Usage: $0 <hours>"
    exit 1
fi

if [ "$1" -lt 2 ]; then
    echo "Minimum 2 hours"
    exit 1
fi

hours="$1"
minutes=$((hours * 60))

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
valid_dirs="crs|tnslsnr|kfod|asm|rdbms"

homes=$("$adrci" exec="SET BASE $diag_dest; SHOW HOMES" | grep -E '^diag/' | grep -E "^diag/($valid_dirs)/")

if [ -z "$homes" ]; then
    echo "No ADR homes found"
    exit 0
fi

cmds="SET BASE $diag_dest"
while IFS= read -r home; do
    cmds="$cmds; SET HOMEPATH $home; PURGE -AGE $minutes"
done <<< "$homes"

"$adrci" exec="$cmds"
