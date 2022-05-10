LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -fPIC -std=c99
LOCAL_LDFLAGS += -fPIC 
LOCAL_LDLIBS := -llog

LOCAL_MODULE    := enable_debug_menu
LOCAL_SRC_FILES := enable_debug_menu.c

include $(BUILD_SHARED_LIBRARY)
