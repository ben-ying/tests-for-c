cmake_minimum_required(VERSION 3.8)
project(WebSocket_server)

set(CMAKE_CXX_STANDARD 11)
set(APP_DIR tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/app/tsd/websocket/mib3)
set(SRC_DIR tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/src/tsd/websocket/mib3)

set(SOURCE_FILES
        ${APP_DIR}/WebSocketServer.cpp
        ${SRC_DIR}/WebSocketTest.cpp
        ${SRC_DIR}/WebSocketTest.hpp)

INCLUDE_DIRECTORIES("tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/app")
INCLUDE_DIRECTORIES("tsd-websocket-mib3/1/workspace/tsd.websocket.mib3/src")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.audioclient.app.mib3/audio/public")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.audioclient.app.mib3/audio/src")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.sds.android/public")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.sds.android.common/public")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.sds.android.common/src")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.audioclient.app.mib3/audio/public")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.audioclient.app.mib3/audio/src")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.audioclient.app.mib3/common/public")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.audioclient.app.mib3/common/src")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.audioclient.app.mib3/config/src")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.audioclient.app.mib3/config/public")
INCLUDE_DIRECTORIES("tsd-audioclient-app-mib3/1/workspace/tsd.audioclient.app.mib3/main/src")
INCLUDE_DIRECTORIES("tsd-dataformats/1/workspace/tsd.dataformats/src")
INCLUDE_DIRECTORIES("tsd-dataformats/1/workspace/tsd.dataformats/public")
#INCLUDE_DIRECTORIES("tsd.sds.android/public/tsd/sds/android/core/backend/playback")
#INCLUDE_DIRECTORIES("tsd.sds.android.common/public/tsd/sds/android/common")
#INCLUDE_DIRECTORIES("tsd.sds.android.common/src/tsd/sds/android/common")

add_executable(WebSocket_server ${SOURCE_FILES})

target_link_libraries(WebSocket_server /usr/lib/libuWS.so)
#target_link_libraries(WebSocket_server crypto)
target_link_libraries(WebSocket_server ssl)
target_link_libraries(WebSocket_server z)
target_link_libraries(WebSocket_server pthread)