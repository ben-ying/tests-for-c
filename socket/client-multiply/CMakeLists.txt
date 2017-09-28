cmake_minimum_required(VERSION 3.8)
project(client_multiply)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.c
        parson.c)

add_executable(client_multiply ${SOURCE_FILES})
target_link_libraries(client_multiply /home/ben/app/tests-for-c/socket/client-multiply/libjsmn.a)


