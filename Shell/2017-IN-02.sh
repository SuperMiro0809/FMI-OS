#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <username>"
    exit 1
fi

username="$1"

count=$(ps -u "$username" --no-headers | wc -l)

echo "Users with more processes than $username (more than $count processes): "
ps -e -o user --no-headers | awk -v cnt="$count" '
{
    all[$1]+=1
}
END{
    for (u in all)
        if ( all[u] > cnt )
            print u
}
'

avg=$( ps -e -o time --no-headers | awk -F: '
{
    seconds += $1*3600 + $2*60 + $3
    count++
}
END{
    avg = seconds/count
    printf "%02d:%02d:%02d\n", avg/3600, (avg%3600)/60, avg%60
}')

echo "Average time: $avg"

avg_seconds=$( ps -e -o time --no-headers | awk -F: '{ s+=$1*3600 + $2*60 + $3; c++ } END{ print s/c }' )

ps -u "$username" -o pid,time --no-headers | while read -r pid time; do
    proc_seconds=$( echo "$time" | awk -F: '{ print $1*3600 + $2*60 + $3 }' )
    if [ $(echo "$proc_seconds > $avg_seconds * 2" | bc) -eq 1 ]; then
        echo "Killing PID $pid (time: $time)"
        kill "$pid"
    fi
done
