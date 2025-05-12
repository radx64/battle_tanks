#!/bin/bash

set -e
./tests.sh
./coverage.sh
./valgrind.sh
