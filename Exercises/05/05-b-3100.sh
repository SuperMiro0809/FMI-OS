#!/bin/bash

read -p "Enter username: " username

count=$(w | grep -c "^$username ")
echo "User has $count active sessions"
