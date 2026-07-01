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

file1=$(sed 's/^#.*//;s/ *#.*//' "$file")

getvalue() {
	echo "$file1" | grep "$1" | sed "s/$1//" | sed -E 's/^ *//;s/ *$//'
}

cmd=$(getvalue 'CMD')
args=$(getvalue 'ARGS' | tr -d '[]')
env=$(getvalue 'ENV' | tr -d '{}')
workdir=$(getvalue 'WORKDIR')

if [ ! -z "$env" ]; then
	echo "$env" | sed -E 's/: /=/g' | awk -F, '{print $0}'
fi

command="$cmd"

if [ ! -z "$args" ]; then
	command="${command} $(echo "$args" | sed 's/",/"/g' | tr -d '"')"
fi

$command
