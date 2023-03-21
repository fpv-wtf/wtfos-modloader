LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -fPIC -std=c99 -O3
LOCAL_LDFLAGS += -fPIC 
LOCAL_LDLIBS := -llog 

LOCAL_MODULE    := menu_hook 
LOCAL_SRC_FILES := menu_hook.c apps.c inih/ini.c

include $(BUILD_SHARED_LIBRARY)
