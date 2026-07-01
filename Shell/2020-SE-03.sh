#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <repo> <package>"
    exit 1
fi

repo="$1"
pkg="$2"

name=$(basename "$pkg")
version=$(cat "$pkg/version")
archive=$(mktemp)

tar -cJf "$archive" -C "$pkg/tree" .

hash=$(sha256sum "$archive" | cut -c1-64)

existing_hash=$(grep "^$name-$version " "$repo/db" | awk '{print $2}')

if [ -n "$existing_hash" ]; then
    rm "$repo/packages/$existing_hash.tar.xz"
    grep -v "^$name-$version " "$repo/db" > /tmp/db_tmp
else
    cp "$repo/db" /tmp/db_tmp
fi

cp "$archive" "$repo/packages/$hash.tar.xz"
echo "$name-$version $hash" >> /tmp/db_tmp
sort /tmp/db_tmp > "$repo/db"

rm "$archive" /tmp/db_tmp
