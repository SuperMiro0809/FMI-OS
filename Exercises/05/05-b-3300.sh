#!/bin/bash

read -p "First file: " file1
read -p "Second file: " file2
read -p "Third file: " file3

paste "$file1" "$file2" | sort > "$file3"
