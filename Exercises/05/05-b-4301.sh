#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 <adress_book> <full_name> <nickname>"
    exit 1
fi

book=$1
fullname=$2
nickname=$3

matches=$( grep "$fullname" /etc/passwd | cut -d: -f1 )
count=$( echo "$matches" | wc -l )

if [ $count -eq 1 ]; then
    username=$matches
else 
    echo "Found $count matches"
    i=1

    echo "$matches" | while read -r user; do
        echo "$i) $user"
        i=$(( i + 1 ))
    done

    read -p "Enter number (0 for cancel): " num

    if [ $num -eq 0 ]; then
        echo "Canceled"
        exit 0
    fi

    username=$(echo "$matches" | head -"$num" | tail -1)
fi

echo "$nickname $username" >> "$book"
