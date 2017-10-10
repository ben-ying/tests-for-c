CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(${MODULE_NAME})

SET(${MODULE_NAME}_SOURCES
        src/tsd/websocket/mib3/WebSocketTest.cpp
        src/tsd/websocket/mib3/WebSocketTest.hpp
        src/tsd/websocket/mib3/Asio.h
        src/tsd/websocket/mib3/Backend.h
        src/tsd/websocket/mib3/Epoll.cpp
        src/tsd/websocket/mib3/Epoll.h
        src/tsd/websocket/mib3/Extensions.cpp
        src/tsd/websocket/mib3/Extensions.h
        src/tsd/websocket/mib3/Group.cpp
        src/tsd/websocket/mib3/Group.h
        src/tsd/websocket/mib3/HTTPSocket.cpp
        src/tsd/websocket/mib3/HTTPSocket.h
        src/tsd/websocket/mib3/Hub.cpp
        src/tsd/websocket/mib3/Hub.h
        src/tsd/websocket/mib3/Libuv.h
        src/tsd/websocket/mib3/Networking.cpp
        src/tsd/websocket/mib3/Networking.h
        src/tsd/websocket/mib3/Node.cpp
        src/tsd/websocket/mib3/Node.h
        src/tsd/websocket/mib3/Socket.cpp
        src/tsd/websocket/mib3/Socket.h
        src/tsd/websocket/mib3/uWS.h
        src/tsd/websocket/mib3/WebSocket.cpp
        src/tsd/websocket/mib3/WebSocket.h
        )

BUILD_ARTIFACT(lib ${${MODULE_NAME}_SOURCES})

MODULE_DEPENDENCIES(PUBLIC
        tsd.common
        tsd.common.checksums
        tsd.common.ini
        ext.openssl
        )

MODULE_INCLUDE_DIRECTORIES(PUBLIC "${MODULE_PATH}/src")

IF (TSD_BUILDENV_ENABLE_APP_TARGETS)
    INCLUDE_DIRECTORIES("${MODULE_PATH}/src")
ENDIF ()

ADD_SUBDIRECTORY(app)
