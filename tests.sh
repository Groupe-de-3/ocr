#!/bin/sh
set -e

if [[ "all" == "$1" ]]
then
    make -sj build-all-tests
    make -sj run-debug-tests
else
    files=""
    targets=""
    for target in "$@" 
    do
        files="${files} lib__${target}_tests.so"
        targets="${targets} build-debug-__${target}_tests"
    done

    make -sj $targets
    make -sj build-debug-tests
    ./build/debug/tests.out $files
fi

