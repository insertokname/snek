#!/usr/bin/env fish

# this is a simple script i use to test out performance changes between versions

test (basename $PWD) != build && cd ./build

rm -rf * \
    && cmake .. \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DCMAKE_CXX_FLAGS="-fno-omit-frame-pointer" \
        -DSNEK_ALGORITHM=ON \
        -DSNEK_PERF=ON \
    && cmake --build . --parallel \
    && time perf record -F 99 --call-graph dwarf -- ./src/snek \
    && perf script | stackcollapse-perf.pl | flamegraph.pl > flame.svg