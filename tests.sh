#!/bin/bash

set -e
mkdir -p build
cd build
build_dir=${PWD}

cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON ..
ninja battle_tanks_testing_engine
ninja battle_tanks_testing_gui

cd $build_dir/tests/engine
./battle_tanks_testing_engine
cd $build_dir/tests/gui
./battle_tanks_testing_gui
