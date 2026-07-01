#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <dir> <history>"
    exit 1
fi

dir="$1"
his="$2"

dates=$(find "$dir" -maxdepth 1 -type f | while read -r file; do
    date=$(basename "$file" .parquet | sed -E 's/.*Forecast_//;s/_[0-9]+//' | awk -F. '{ printf "%02d.%02d.%s\n", $1, $2, $3 }')
    echo $date
done | sort | uniq -c | awk '$1 > 1 { print $2 }')

while IFS= read -r date; do
    latest=1
    while read -r file; do
        d=$(basename "$file" .parquet | sed -E 's/.*Forecast_//;s/_[0-9]+//' | awk -F. '{ printf "%02d.%02d.%s\n", $1, $2, $3 }')
        
        if [ "$date" == "$d" ]; then
            hash=$(sha1sum "$file" | awk '{ print $1 }')
            l=$(cat "$his" | grep -n "$hash" | awk -F: '{print $1}')

            if [ -n "$l" ] && [ "$l" -gt "$latest" ]; then
                latest="$l"
            fi
        fi
    done < <(find "$dir" -maxdepth 1 -type f)
    
    while read -r file; do
        d=$(basename "$file" .parquet | sed -E 's/.*Forecast_//;s/_[0-9]+//' | awk -F. '{ printf "%02d.%02d.%s\n", $1, $2, $3 }')
 
        if [ "$date" == "$d" ]; then
            hash=$(sha1sum "$file" | awk '{ print $1 }')
            l=$(cat "$his" | grep -n "$hash" | awk -F: '{print $1}')
             
            if [ "$l" -lt "$latest" ]; then
                 rm "$file"
            fi
        fi
    done < <(find "$dir" -maxdepth 1 -type f)
done < <(echo "$dates")
