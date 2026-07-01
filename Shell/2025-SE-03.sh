#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <dir> [dir2 ...]"
    exit 1
fi

if [ -z "$REGISTRY_FILE" ] || [ -z "$REPORTS_DIR" ]; then
    echo "REGISTRY_FILE and REPORTS_DIR must be set"
    exit 1
fi

mkdir -p "$REPORTS_DIR"
touch "$REGISTRY_FILE"

timestamp=$(date '+%Y-%m-%d-%H-%M-%S')
report="$REPORTS_DIR/$timestamp.report"

new_files=""
unchanged_files=""
modified_files=""

new_registry=""

for dir in "$@"; do
    while IFS= read -r file; do
        abspath=$(realpath "$file")
        hash=$(sha256sum "$file" | cut -c1-64)

        registry_hash=$(grep " $abspath$" "$REGISTRY_FILE" | awk '{print $1}')

        if [ -z "$registry_hash" ]; then
            new_files="${new_files}$abspath\n"
        elif [ "$hash" = "$registry_hash" ]; then
            unchanged_files="${unchanged_files}$abspath\n"
        else
            modified_files="${modified_files}$abspath\n"
        fi

        new_registry="${new_registry}$hash $abspath\n"
    done < <(find "$dir" -type f)
done

{
    echo "new:"
    printf "%b" "$new_files"
    echo "unchanged:"
    printf "%b" "$unchanged_files"
    echo "modified:"
    printf "%b" "$modified_files"
} > "$report"

printf "%b" "$new_registry" > "$REGISTRY_FILE"
