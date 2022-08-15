#!/usr/bin/bash

set -e

cd ../../build
qmake ..
compiledb make -j4
./learning_graph
