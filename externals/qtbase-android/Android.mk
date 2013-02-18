LOCAL_PATH := $(call my-dir)

PROJECT_ROOT_RELATIVE := ../../../platforms/android/OsmAnd
OSMAND_QTBASE_ROOT_RELATIVE := .
OSMAND_QTBASE_ROOT := $(LOCAL_PATH)/$(OSMAND_QTBASE_ROOT_RELATIVE)
OSMAND_QTBASE_RELATIVE := ./upstream.patched
OSMAND_QTBASE := $(LOCAL_PATH)/$(OSMAND_QTBASE_RELATIVE)

ifeq ($(OSMAND_BUILDING_NEON_LIBRARY),true)
	OSMAND_QTBASE_SUFFIX := .$(TARGET_ARCH_ABI)-neon
	MODULE_SUFFIX := _neon
else
	OSMAND_QTBASE_SUFFIX := .$(TARGET_ARCH_ABI)
	MODULE_SUFFIX :=
endif

include $(CLEAR_VARS)
LOCAL_ARM_NEON := $(OSMAND_BUILDING_NEON_LIBRARY)
LOCAL_MODULE := Qt5Core$(MODULE_SUFFIX)
LOCAL_SRC_FILES := $(OSMAND_QTBASE_RELATIVE)$(OSMAND_QTBASE_SUFFIX)/lib/lib$(LOCAL_MODULE).so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_NEON := $(OSMAND_BUILDING_NEON_LIBRARY)
LOCAL_MODULE := Qt5Network$(MODULE_SUFFIX)
LOCAL_SRC_FILES := $(OSMAND_QTBASE_RELATIVE)$(OSMAND_QTBASE_SUFFIX)/lib/lib$(LOCAL_MODULE).so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_NEON := $(OSMAND_BUILDING_NEON_LIBRARY)
LOCAL_MODULE := Qt5Xml$(MODULE_SUFFIX)
LOCAL_SRC_FILES := $(OSMAND_QTBASE_RELATIVE)$(OSMAND_QTBASE_SUFFIX)/lib/lib$(LOCAL_MODULE).so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_NEON := $(OSMAND_BUILDING_NEON_LIBRARY)
LOCAL_MODULE := Qt5Sql$(MODULE_SUFFIX)
LOCAL_SRC_FILES := $(OSMAND_QTBASE_RELATIVE)$(OSMAND_QTBASE_SUFFIX)/lib/lib$(LOCAL_MODULE).so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_NEON := $(OSMAND_BUILDING_NEON_LIBRARY)
LOCAL_MODULE := Qt5Concurrent$(MODULE_SUFFIX)
LOCAL_SRC_FILES := $(OSMAND_QTBASE_RELATIVE)$(OSMAND_QTBASE_SUFFIX)/lib/lib$(LOCAL_MODULE).so
include $(PREBUILT_SHARED_LIBRARY)