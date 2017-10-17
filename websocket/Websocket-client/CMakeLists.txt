cmake_minimum_required(VERSION 3.8)
project(Websocket_client)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        json.hpp)

add_executable(Websocket_client ${SOURCE_FILES})

target_link_libraries(Websocket_client /usr/lib/libuWS.so)
target_link_libraries(Websocket_client ssl)
target_link_libraries(Websocket_client z)