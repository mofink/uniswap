#!/bin/sh

cmake -S . -B build
cd build
cmake --build .

exit