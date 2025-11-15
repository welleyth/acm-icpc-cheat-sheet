#!/bin/bash
for((i = 1; ; ++i)); do
    echo "Seed: $i"
    ./cmake-build-debug/gen $i > test
    diff -w <(./cmake-build-debug/CF < test 2> /dev/null) <(./cmake-build-debug/brute < test 2> /dev/null) &>/dev/null || break
done

cat test

