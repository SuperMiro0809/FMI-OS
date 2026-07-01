#!/bin/bash

read -p "Enter file: " file
read -p "Enter string: " str

grep -q "$str" "$file"
echo "$?"
