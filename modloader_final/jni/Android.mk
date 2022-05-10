LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -fPIC -std=c99
LOCAL_LDFLAGS += -fPIC 
LOCAL_LDLIBS := -llog

LOCAL_MODULE    := modloader_final
LOCAL_SRC_FILES := modloader_final.c

include $(BUILD_SHARED_LIBRARY)