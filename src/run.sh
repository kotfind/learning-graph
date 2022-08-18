#!/usr/bin/bash

set -e

cd ../build
cmake \
    -DCMAKE_CXX_COMPILER_LAUNCHER="ccache;distcc" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=true \
    -DCMAKE_BUILD_TYPE=Debug \
    ../src
cmake --build . -- -j
./learning_graph
