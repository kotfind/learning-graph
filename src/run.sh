#!/usr/bin/bash

set -e

cd ../build
qmake6 ..
compiledb make -j4
./learning_graph
