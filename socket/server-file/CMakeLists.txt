cmake_minimum_required(VERSION 3.8)
project(server_file)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.c)

add_executable(server_file ${SOURCE_FILES})