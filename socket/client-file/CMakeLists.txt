cmake_minimum_required(VERSION 3.8)
project(client_file)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.c)

add_executable(client_file ${SOURCE_FILES})