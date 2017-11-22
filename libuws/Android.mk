LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES += exceptions
LOCAL_CPPFLAGS += -fexceptions

#LOCAL_CFLAGS := -Wall -Werror

LOCAL_C_INCLUDES := \
    external/boringssl/include \
    external/zlib \

LOCAL_SRC_FILES := \
    src/Epoll.cpp \
    src/Extensions.cpp \
    src/Group.cpp \
    src/HTTPSocket.cpp \
    src/Hub.cpp \
    src/Networking.cpp \
    src/Node.cpp \
    src/Socket.cpp \
    src/WebSocket.cpp \

LOCAL_SHARED_LIBRARIES := libssl libcrypto libz

LOCAL_EXPORT_C_INCLUDE_DIRS := \
    $(LOCAL_PATH)/include \
    $(LOCAL_C_INCLUDES) \

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDE_DIRS)

LOCAL_MODULE := libuws

include $(BUILD_SHARED_LIBRARY)
