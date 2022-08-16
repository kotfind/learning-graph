#!/usr/bin/bash

set -e

cd ../build
qmake6 ..
compiledb make -j
./learning_graph
