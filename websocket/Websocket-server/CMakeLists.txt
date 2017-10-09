cmake_minimum_required(VERSION 3.8)
project(Websocket_server)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp)

add_executable(Websocket_server ${SOURCE_FILES})
target_link_libraries(Websocket_server /usr/lib/libuWS.so)
target_link_libraries(Websocket_server ssl)
target_link_libraries(Websocket_server z)
target_link_libraries(Websocket_server pthread)