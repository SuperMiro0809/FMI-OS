#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <num>"
    exit 1
fi

num="$1"

ps -e -o user,pid,rss --no-headers | sort -k1 | awk '
{
    rss[$1] += $3
    if ($3 > max_rss[$1]) {
        max_rss[$1] = $3
        max_pid[$1] = $2
    }
}

END{
    for (u in rss)
        print u, rss[u], max_pid[u]
}
' | while read -r user total pid; do
    echo "$user: $total"
    if [ "$total" -gt "$num" ]; then
        echo "Killing PID $pid ($user exceeds limit)"
        kill "$pid"
    fi
done
