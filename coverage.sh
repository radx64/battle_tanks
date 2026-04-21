#!/bin/bash

mkdir -p build_tests
cd build_tests
cmake -GNinja -DCMAKE_BUILD_TYPE=Coverage -DBUILD_TESTS=ON ..
ninja -v battle_tanks_testing_gui_coverage
ninja -v battle_tanks_testing_engine_coverage
