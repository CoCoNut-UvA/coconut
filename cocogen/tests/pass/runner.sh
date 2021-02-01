#!/usr/bin/env bash

test -n "$1" || { echo "First arguments needs to be cocogen executable"; exit 1; }
cocogen="$1"
test_files=$(ls *.ccn)
error=0
tmpdir="/tmp/cocogen-pass-test"
mkdir -p ${tmpdir}

for f in ${test_files}; do
    echo "Testing: $f"
    "$1" "$f" >/dev/null
    if [[ $? -ne 0 ]]; then
        echo ""
        ((error += 1))
    fi
done

echo "---------------------------------------------"
rm -rf ccngen/
test $error -eq 0 || { echo "Test failed with $error error(s)"; exit 1; }
echo "Succes!"
