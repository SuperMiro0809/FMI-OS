#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <file> <dir>"
    exit 1
fi

csv="$1"
dir="$2"

if [ ! -d "$dir" ]; then
    echo "$dir must be a directory"
    exit 1
fi

getparameter() {
    cat "$1" | grep "$2" | awk -F: '{ print $2 }' | sed 's/^ *//;s/ *$//'
}

echo 'hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key' > "$csv"

find "$dir" -maxdepth 1 -type f -name '*.log' | while read -r file; do
    host=$(basename "$file" .log)
    phy=$(getparameter "$file" 'Maximum Physical Interfaces')
    vlans=$(getparameter "$file" 'VLANs')
    hosts=$(getparameter "$file" 'Inside Hosts')
    failover=$(getparameter "$file" 'Failover')
    vpn=$(getparameter "$file" 'VPN-3DES-AES')
    peers=$(getparameter "$file" '\*Total VPN Peers')
    ports=$(getparameter "$file" 'VLAN Trunk Ports')
    license=$(cat "$file" | grep 'This platform has' | sed -E 's/.* has (a|an) (.*) license\./\2/')
    serial=$(getparameter "$file" 'Serial Number')
    key=$(getparameter "$file" 'Running Activation Key')

    echo "$host,$phy,$vlans,$hosts,$failover,$vpn,$peers,$ports,$license,$serial,$key"
done >> "$csv"
