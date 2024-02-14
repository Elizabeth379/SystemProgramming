#!/bin/bash

script_dir="$(dirname "$0")"

if [ -z "$1" ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

if [ ! -f "$1" ]; then
    echo "File not found: $1"
    exit 1
fi

input=$(cat "$1")

if ! echo "$input" | grep -q -E '(\.\s+[a-z])|(!\s+[a-z])|(\?\s+[a-z])|^[a-z]'; then
    echo "In that file all good"
    exit 0
fi

result=$(echo "$input" | sed -e 's/\.\s\{1,\}[a-z]/\U&/g' \
                              -e 's/!\s\{1,\}[a-z]/\U&/g' \
                              -e 's/\?\s\{1,\}[a-z]/\U&/g' \
                              -e 's/^[a-z]/\U&/')

echo "$result"

echo "$result" > "$script_dir/result2"