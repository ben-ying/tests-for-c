CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(${MODULE_NAME})

SET(${MODULE_NAME}_SOURCES
        src/tsd/websocket/mib3/WebSocketTest.cpp
        src/tsd/websocket/mib3/WebSocketTest.hpp
        src/tsd/websocket/uws/Asio.h
        src/tsd/websocket/uws/Backend.h
        src/tsd/websocket/uws/Epoll.cpp
        src/tsd/websocket/uws/Epoll.h
        src/tsd/websocket/uws/Extensions.cpp
        src/tsd/websocket/uws/Extensions.h
        src/tsd/websocket/uws/Group.cpp
        src/tsd/websocket/uws/Group.h
        src/tsd/websocket/uws/HTTPSocket.cpp
        src/tsd/websocket/uws/HTTPSocket.h
        src/tsd/websocket/uws/Hub.cpp
        src/tsd/websocket/uws/Hub.h
        src/tsd/websocket/uws/Libuv.h
        src/tsd/websocket/uws/Networking.cpp
        src/tsd/websocket/uws/Networking.h
        src/tsd/websocket/uws/Node.cpp
        src/tsd/websocket/uws/Node.h
        src/tsd/websocket/uws/Socket.cpp
        src/tsd/websocket/uws/Socket.h
        src/tsd/websocket/uws/uWS.h
        src/tsd/websocket/uws/WebSocket.cpp
        src/tsd/websocket/uws/WebSocket.h
        )

BUILD_ARTIFACT(lib ${${MODULE_NAME}_SOURCES})

MODULE_DEPENDENCIES(PUBLIC
        tsd.common
        tsd.common.checksums
        tsd.common.ini
        ext.openssl
#        ext.libuWS
        )

MODULE_INCLUDE_DIRECTORIES(PUBLIC "${MODULE_PATH}/src")

#IF (TSD_BUILDENV_ENABLE_APP_TARGETS)
#    INCLUDE_DIRECTORIES("${MODULE_PATH}/src")
#ENDIF ()

ADD_SUBDIRECTORY(app)

