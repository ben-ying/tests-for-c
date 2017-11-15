cmake_minimum_required(VERSION 3.8)
project(mysocket)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    uWs/Asio.h
    uWs/Backend.h
    uWs/Epoll.cpp
    uWs/Epoll.h
    uWs/Extensions.cpp
    uWs/Extensions.h
    uWs/Group.cpp
    uWs/Group.h
    uWs/HTTPSocket.cpp
    uWs/HTTPSocket.h
    uWs/Hub.cpp
    uWs/Hub.h
    uWs/Libuv.h
    uWs/Networking.cpp
    uWs/Networking.h
    uWs/Node.cpp
    uWs/Node.h
    uWs/Socket.cpp
    uWs/Socket.h
    uWs/uWS.h
    uWs/WebSocket.cpp
    uWs/WebSocket.h
    uWs/WebSocketProtocol.h
    Android.mk
    json.hpp
    main.cpp
    README.auditd
    README.property
    socket.json)

add_executable(mysocket ${SOURCE_FILES})