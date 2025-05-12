#!/bin/bash

run_valgrind() {
    valgrind --leak-check=full --suppressions=valgrind.supp -s --error-exitcode=1 $1
}

set -e
mkdir -p build
cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON ..
ninja battle_tanks_testing_engine
ninja battle_tanks_testing_gui
cd ..
run_valgrind ./build/tests/engine/battle_tanks_testing_engine
run_valgrind ./build/tests/gui/battle_tanks_testing_gui
