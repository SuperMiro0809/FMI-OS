#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <src> <dst>"
    exit 1
fi

src="$1"
dst="$2"

if [ -e "$dst" ]; then
    echo "$dst must not exist"
    exit 1
fi

mkdir -p "$dst"

clean() {
    echo "$1" | sed -E 's/^ *//;s/ *$//' | sed 's/  */ /'
}

find "$src" -type f -name "*.jpg" | while read -r file; do
    name=$(clean "$(basename "$file" .jpg)")
    title=$(clean "$(echo "$name" | sed -E 's/ *\([^)]*\)//g')")
    album=$(clean "$(echo "$name" | grep -oE '\([^)]*\)' | tail -1 | tr -d '()')")
    date=$(stat -c '%y' "$file" | cut -c1-10)   
    hash=$(sha256sum "$file" | cut -c1-16)

    if [ -z "$album" ]; then
        album='misc'    
    fi

    mkdir -p "$dst/images"
    cp "$file" "$dst/images/$hash.jpg"

    mkdir -p "$dst/by-title"
    ln -s "../images/$hash.jpg" "$dst/by-title/$title.jpg"

    mkdir -p "$dst/by-date/$date/by-title"
    ln -s "../../../images/$hash.jpg" "$dst/by-date/$date/by-title/$title.jpg"

    mkdir -p "$dst/by-album/$album/by-title"
    ln -s "../../../images/$hash.jpg" "$dst/by-album/$album/by-title/$title.jpg"

    mkdir -p "$dst/by-album/$album/by-date/$date/by-title"
    ln -s "../../../../../images/$hash.jpg" "$dst/by-album/$album/by-date/$date/by-title/$title.jpg"

    mkdir -p "$dst/by-date/$date/by-album/$album/by-title"
    ln -s "../../../../../images/$hash.jpg" "$dst/by-date/$date/by-album/$album/by-title/$title.jpg"
done
