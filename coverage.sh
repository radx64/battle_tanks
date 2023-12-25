#!/bin/bash
mkdir build
cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=Coverage -DBUILD_TESTS=ON ..
ninja -v battle_tanks_testing_gui_coverage
ninja -v battle_tanks_testing_math_coverage