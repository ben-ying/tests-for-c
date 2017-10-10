cmake_minimum_required(VERSION 3.8)
project(WebSocket_server)

set(CMAKE_CXX_STANDARD 11)
set(APP_DIR tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/app/tsd/websocket/mib3)
set(SRC_DIR tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/src/tsd/websocket/mib3)

set(SOURCE_FILES
        ${APP_DIR}/WebSocketServer.cpp
        ${SRC_DIR}/Asio.h
        ${SRC_DIR}/Backend.h
        ${SRC_DIR}/Epoll.cpp
        ${SRC_DIR}/Epoll.h
        ${SRC_DIR}/Extensions.cpp
        ${SRC_DIR}/Extensions.h
        ${SRC_DIR}/Group.cpp
        ${SRC_DIR}/Group.h
        ${SRC_DIR}/HTTPSocket.cpp
        ${SRC_DIR}/HTTPSocket.h
        ${SRC_DIR}/Hub.cpp
        ${SRC_DIR}/Hub.h
        ${SRC_DIR}/Libuv.h
        ${SRC_DIR}/Hub.h
        ${SRC_DIR}/Networking.cpp
        ${SRC_DIR}/Networking.h
        ${SRC_DIR}/Node.cpp
        ${SRC_DIR}/Node.h
        ${SRC_DIR}/Socket.cpp
        ${SRC_DIR}/Socket.h
        ${SRC_DIR}/uWS.h
        ${SRC_DIR}/WebSocket.cpp
        ${SRC_DIR}/WebSocket.h
        ${SRC_DIR}/WebSocketProtocol.h
        ${SRC_DIR}/WebSocketTest.cpp
        ${SRC_DIR}/WebSocketTest.hpp)

INCLUDE_DIRECTORIES("tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/app")
INCLUDE_DIRECTORIES("tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/src")

add_executable(WebSocket_server ${SOURCE_FILES})

#target_link_libraries(WebSocket_server /usr/lib/libuWS.so)
target_link_libraries(WebSocket_server ssl)
target_link_libraries(WebSocket_server z)
target_link_libraries(WebSocket_server pthread)