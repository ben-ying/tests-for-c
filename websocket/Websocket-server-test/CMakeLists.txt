cmake_minimum_required(VERSION 3.8)
project(WebSocket_server_test)

set(CMAKE_CXX_STANDARD 11)
set(APP_DIR tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/app/tsd/websocket/mib3)
set(SRC_DIR tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/src/tsd/websocket/mib3)

set(SOURCE_FILES
        ${APP_DIR}/WebSocketServer.cpp
        ${SRC_DIR}/WebSocketTest.cpp
        ${SRC_DIR}/WebSocketTest.hpp)

INCLUDE_DIRECTORIES("tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/app")
INCLUDE_DIRECTORIES("tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/src")

add_executable(WebSocket_server_test ${SOURCE_FILES})

target_link_libraries(WebSocket_server_test /usr/lib/libuWS.so)
#target_link_libraries(WebSocket_server_test crypto)
target_link_libraries(WebSocket_server_test ssl)
target_link_libraries(WebSocket_server_test z)
target_link_libraries(WebSocket_server_test pthread)
