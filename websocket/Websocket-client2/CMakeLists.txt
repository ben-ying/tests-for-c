cmake_minimum_required(VERSION 3.8)
project(Websocket_client2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp)

add_executable(Websocket_client2 ${SOURCE_FILES})

target_link_libraries(Websocket_client2 /usr/lib/libuWS.so)
target_link_libraries(Websocket_client2 ssl)
target_link_libraries(Websocket_client2 z)