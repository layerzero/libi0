#!/bin/env bash

# A script to expand #PRINTSTR macro in C0 program.
# This script expand #PRINTSTR to calls to `put1()`s and
# print the processed source code to STDOUT

# example.c
# #include "stdio.h"
#
# void main()
# {
#     #PRINTSTR hello world!
#     wrln();
#     commit;
# }

# after processing by ./strsplitter.bash example.c:
# #include "stdio.h"
#
# void main()
# {
# put1('h');put1('e');put1('l');put1('l');put1('o');put1(' ');put1('w');put1('o');put1('r');put1('l');put1('d');put1('!');
#     wrln();
#     commit;
# }


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
    elif [ "${nospace:0:8}" == "#PRINTLN" ]; then
        str=${nospace:9}
        for (( i=0; i<${#str}; i++ )); do
            echo -n "put1('${str:$i:1}');"
        done
        echo "wrln();"
    else
        echo "$line"
    fi
done <$file
