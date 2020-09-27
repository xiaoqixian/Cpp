cmake_minimum_required(VERSION 1.0)

project(cmake)

add_executable(cmake main.cpp)

add_library(hello_lib STATIC ${PROJECT_SOURCE_DIR}/include/hello.cpp)

target_include_directories(hello_lib
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(cmake
    PRIVATE
        hello_lib
)
