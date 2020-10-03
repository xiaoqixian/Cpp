cmake_minimum_required(VERSION 2.6)

project(mempool)

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -Wall")
file(GLOB SOURCES "*.cpp")
add_executable(mempool ${SOURCES})

