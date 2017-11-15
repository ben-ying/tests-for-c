LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES += exceptions
LOCAL_CPPFLAGS += -fexceptions

LOCAL_MODULE := websocket

LOCAL_SRC_FILES := main.cpp

LOCAL_C_INCLUDES := external/boringssl/include 

LOCAL_SHARED_LIBRARIES := libssl libcrypto libz

LOCAL_SRC_FILES := \
    main.cpp \
    uWs/Epoll.cpp \
    uWs/Extensions.cpp \
    uWs/Group.cpp \
    uWs/HTTPSocket.cpp \
    uWs/Hub.cpp \
    uWs/Networking.cpp \
    uWs/Node.cpp \
    uWs/Socket.cpp \
    uWs/WebSocket.cpp \

include $(BUILD_EXECUTABLE)

