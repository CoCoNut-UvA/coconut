#!/usr/bin/env bash

## Small runner that checks if the compiler gives an exit code != 0 for the files
## found in this directory. If there is a <filename>.out file in the expected 
## directory, the output is checked with the expected file via `diff`.
## Run this via ctest in the corresponding build dir.

test -n "$1" || { echo "First arguments needs to be cocogen executable"; exit 1; }
cocogen="$1"
test_files=$(ls *.ccn)
error=0
tmpdir="/tmp/cocogen-fail-test"
expected="./expected"
mkdir -p ${tmpdir}

for f in ${test_files}; do
    echo "Testing: $f"
    outfile=$(basename "$f" .ccn).out
    echo "Testing: $f"
    "$1" "$f" >/dev/null 2>"${tmpdir}/${outfile}"
    if [[ $? -eq 0 ]]; then
        printf "\tZero exit code for file: $f\n"
        ((error += 1))
    elif [[ -f "./${expected}/${outfile}" ]]; then
        diff -b "${expected}/${outfile}" "${tmpdir}/${outfile}"
        test $? -ne 0 && ((error += 1))
    fi
done

echo "---------------------------------------------"
rm -rf ccngen/
test $error -eq 0 || { echo "Test failed with $error error(s)"; exit 1; }
echo "Succes!"
