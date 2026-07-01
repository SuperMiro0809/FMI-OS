#!/bin/bash

read -p "Enter directory name: " dir

files_count=$(find "$dir" -type f | wc -l)
dir_count=$(find "$dir" -mindepth 1 -type d | wc -l)

echo "Files: $files_count"
echo "Dirs: $dir_count"
echo "Total: $(( $files_count + $dir_count ))"
