#!/usr/bin/bash

set -e

cd ../../build
qmake ..
compiledb make
./learning_graph
