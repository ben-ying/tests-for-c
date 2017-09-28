cmake_minimum_required(VERSION 3.8)
project(server_multiply)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.c
        parson.c)

add_executable(server_multiply ${SOURCE_FILES})