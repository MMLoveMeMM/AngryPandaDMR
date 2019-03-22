LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := platinum-jni
LOCAL_SRC_FILES  := com_pumpkin_platinum_DlanCore.cpp \
					PltMediaRendererDelegate.cpp
LOCAL_LDLIBS     += -llog
LOCAL_LDLIBS     += -landroid

#PLT_ROOT := $(LOCAL_PATH)/../../../../../..
#PLT_SRC_ROOT := $(PLT_ROOT)/Source

PLT_ROOT := $(LOCAL_PATH)
PLT_SRC_ROOT := $(PLT_ROOT)/PlatinumKit/Platinum/Source

LOCAL_C_INCLUDES += $(PLT_SRC_ROOT)/Platinum
LOCAL_C_INCLUDES += $(PLT_SRC_ROOT)/Core
LOCAL_C_INCLUDES += $(PLT_SRC_ROOT)/Devices/MediaConnect
LOCAL_C_INCLUDES += $(PLT_SRC_ROOT)/Devices/MediaServer
LOCAL_C_INCLUDES += $(PLT_SRC_ROOT)/Devices/MediaRenderer
LOCAL_C_INCLUDES += $(PLT_SRC_ROOT)/Extras
LOCAL_C_INCLUDES += $(PLT_ROOT)/PlatinumKit/Neptune/Source/Core

LOCAL_CFLAGS += -DNPT_CONFIG_ENABLE_LOGGING

LOCAL_LDLIBS += -L$(PLT_ROOT)/PlatinumKit/Platinum/Build/Targets/arm-android-linux/Release

LOCAL_LDLIBS += -lPlatinum
LOCAL_LDLIBS += -lNeptune
LOCAL_LDLIBS += -laxTLS
LOCAL_LDLIBS += -lPltMediaRenderer

include $(BUILD_SHARED_LIBRARY)
