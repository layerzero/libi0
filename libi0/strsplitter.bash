#!/bin/env bash

# A script to expand #PRINTSTR macro in C0 program.
# This script expand #PRINTSTR to calls to `put1()`s and
# print the processed source code to STDOUT

if [ $# -lt 1 ]; then
    echo "Usage: $0 file"
    exit 1
fi

file=$1

# keep spaces
OLD_IFS="$IFS"
IFS=

while read line; do
    nospace=`echo "$line" | sed 's/^ *//'`
    if [ "${nospace:0:9}" == "#PRINTSTR" ]; then
        str=${nospace:10}
        for (( i=0; i<${#str}; i++ )); do
            echo -n "put1('${str:$i:1}');"
        done
        echo ""
    else
        echo "$line"
    fi
done <$file
