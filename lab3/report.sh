#!/bin/bash
# usage: ./report.sh access.log
log=$1
if [ ! -f "$log" ]; then
    echo "no such file: $log" >&2
    exit 1
fi
total=$(wc -l < "$log")
fails=$(grep -c FAIL "$log")
echo "$total requests, $fails failed"
for i in 0 1 2 3 4 5 6; do
    n=$(grep -c " user$i FAIL " "$log")
    echo "user$i: $n failures"
done
