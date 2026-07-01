#!/bin/bash

echo "<table>"
echo "  <tr>"
echo "      <th>Username</th>"
echo "      <th>group</th>"
echo "      <th>login shell</th>"
echo "      <th>GECOS</th>"
echo "  </tr>"

awk -F: '{
    print " <tr>"
    print "     <td>" $1 "</td>"
    print "     <td>" $4 "</td>"
    print "     <td>" $7 "</td>"
    print "     <td>" $5 "</td>"
    print " </tr>"
}' /etc/passwd

echo "</table>"
