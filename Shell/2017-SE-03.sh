#!/bin/bash

ps -e -o pid,user,rss --no-headers | awk '
{
    all_rss[$2] += $3
    count[$2]++

    if ($3 > max_rss[$2] ) {
        max_rss[$2] = $3
        max_pid[$2] = $1
    }
}
END{
    for (u in all_rss) {
        avg = all_rss[u] / count[u]
        print u": count=" count[u] " rss=" all_rss[u]

        if (max_rss[u] > avg * 2) {
            print "kill " max_pid[u] | "bash"
        }
    }
}'
