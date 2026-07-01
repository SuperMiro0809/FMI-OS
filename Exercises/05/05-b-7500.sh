#!/bin/bash

a=${1:-1}
b=${2:-100}

secret=$(( (RANDOM % (b - a + 1)) + a ))

tries=0

while true; do
    read -p "Guess: " guess
    tries=$(( tries + 1 ))

    if [ $guess -eq $secret ]; then
        echo "Your guess is correct! Number of tries: $tries"
        exit 0
    elif [ $guess -gt $secret ]; then
        echo "smaller"
    else
        echo "bigger"
    fi 
done
