#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <domain> <file>"
    exit 1
fi

domain="$1"
file="$2"

declare -A team_zones
declare -A team_servers

while read -r server zone team; do
    if [ -z "${team_zones[$team]}" ]; then
        team_zones[$team]="$zone" 
        team_servers[$team]="$server"
    else
        team_zones[$team]="${team_zones[$team]}:$zone"
        team_servers[$team]="${team_servers[$team]}:$server"
    fi
done < <(cat "$file")

for k in "${!team_zones[@]}"; do
    echo "; team $k"

    IFS=: read -ra zones <<< "${team_zones[$k]}"
    IFS=: read -ra servers <<< "${team_servers[$k]}"

    for zone in "${zones[@]}"; do
        for server in "${servers[@]}"; do
            echo "$zone IN NS $server.$domain."
        done
    done
done
