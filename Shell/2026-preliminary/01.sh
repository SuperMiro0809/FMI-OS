#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $0 <file>"
	exit 1
fi

file="$1"

if [ ! -f "$file" ]; then
	echo "$file must be a file"
	exit 1
fi

hist=$(mktemp)

content=""

file_reader() {
	local path=$(dirname "$1")
	local full_path=$(realpath "$1")

	if [ ! -f "$1" ]; then
		echo "$1 could not be read!"
		exit 1
	fi

	while read -r line; do
		include=$(echo "$line" | grep -E '^\!include')

		if [ -z "$line" ] || [ -z "$include" ]; then
			content="${content}$line\n"
			continue
		fi

		if [ ! -z "$include" ]; then
			imp=$(echo "$line" | cut -d: -f2 | sed 's/^ *//')
			imp_path=$(realpath "$path/$imp")

			seen_as_parent=$(cat "$hist" | cut -d: -f1 | grep -c "$imp_path")
			seen_as_child=$(cat "$hist" | cut -d: -f2 | grep -c "$full_path")

			if [ "$seen_as_parent" -ne 0  ] && [ "$seen_as_child" -ne 0 ]; then
				echo "Cycle detected"
				exit 1
			fi

			echo "$full_path:$(realpath "$path/$imp")" >> "$hist"
			file_reader "$path/$imp"
		fi 
	done < <(cat "$1")
}

file_reader "$file"
printf "$content"

rm "$hist"
