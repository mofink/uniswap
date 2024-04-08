
cmake_minimum_required(VERSION 3.10)

project(Uniswap)

SET(CMAKE_EXE_LINKER_FLAGS -pthread)

add_executable(Uniswap main.cxx pool.cxx test.cxx arbitrage.cxx)

