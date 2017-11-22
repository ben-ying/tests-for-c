LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_FEATURES += exceptions
LOCAL_CPPFLAGS += -fexceptions

LOCAL_SRC_FILES := main.cpp
LOCAL_MODULE := mysocket
LOCAL_LDLIBS := -luws

LOCAL_SHARED_LIBRARIES := libssl libcrypto libz


include $(BUILD_EXECUTABLE)


