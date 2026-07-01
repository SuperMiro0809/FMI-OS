#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 <pwd_file> <config_out> <cfgdir>"
    exit 1
fi

pwdfile="$1"
configout="$2"
cfgdir="$3"

validate_cfg() {
    local file="$1"
    local errors=""
    local lineno=0

    while IFS= read -r line; do
        lineno=$(( lineno + 1 ))

        if ! echo "$line" | grep -qE '^\s*#|^\s*$|^\s*\{ [^}]*\};\s*$'; then
            errors="${errors}Line $lineno:$line\n"
        fi
    done < "$file"

    echo "$errors"
}

> "$configout"

find "$cfgdir" -type f -name "*.cfg" | while read -r cfg; do
    errors=$(validate_cfg "$cfg")

    if [ -n "$errors" ]; then
        echo "Error in $(basename "$cfg"):"
        echo -e "$errors"
    else
        cat "$cfg" >> "$configout"

        username=$(basename "$cfg" .cfg)
        if ! grep -q "^$username:" "$pwdfile"; then
            password=$(pwgen 16 1)
            hash=$(echo -n "$password" | md5sum | cut -c1-32)
            echo "$username:$hash" >> "$pwdfile"
            echo "$username $password"
        fi
    fi
done
