#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <old_dir> <new_dir>"
    exit 1
fi

olddir="${1%/}"
newdir="${2%/}"

mkdir -p "$newdir"

while IFS= read -r file; do
    declare -A keys
    keys=()

    while IFS== read -r key val; do
        if [ -z "${keys[$key]}" ]; then
            keys[$key]="$val"
        else
            keys[$key]="${keys[$key]}:$val"
        fi
    done < <(cat "$file")
    
    content=""
    for key in "${!keys[@]}"; do
        IFS=: read -ra vals <<< "${keys[$key]}"
        
        if [ "${#vals[@]}" -eq 1 ]; then
            content="${content}$key: ${vals[0]}\n"
        else
            content="${content}$key:\n"
            for val in "${vals[@]}"; do
                content="${content}\t- $val\n"
            done
        fi
    done

    newfile="${file/$olddir/$newdir}"
    newfile="${newfile%.bcf}.bcf2"
    mkdir -p "$(dirname "$newfile")"

    printf "$content" > "$newfile"

done < <(find "$olddir" -type f -name '*.bcf')
