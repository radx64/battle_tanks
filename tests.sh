#!/bin/bash

set -e
mkdir -p build
cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON ..
ninja battle_tanks_testing_engine
ninja battle_tanks_testing_gui
./tests/engine/battle_tanks_testing_engine
./tests/gui/battle_tanks_testing_gui
