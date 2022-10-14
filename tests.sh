#!/bin/sh
set -e

LIBS_PROFILE=debug
EXE_PROFILE=debug

if [[ "all" == "$1" ]]
then
    make -sj all-tests-${LIBS_PROFILE}
    make -sj build-${EXE_PROFILE}-tests
        
    files=$(find ./build/${LIBS_PROFILE} -maxdepth 1 -name "lib__*_tests.so")

    ./build/${EXE_PROFILE}/tests.out ${files}
else
    files=""
    targets=""
    for target in "$@" 
    do
        files="${files} ${PWD}/build/${LIBS_PROFILE}/lib__${target}_tests.so"
        targets="${targets} build-${LIBS_PROFILE}-__${target}_tests"
    done

    make -sj $targets
    make -sj build-${EXE_PROFILE}-tests
    ./build/${EXE_PROFILE}/tests.out ${files}
fi

