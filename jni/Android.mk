LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
OPENCV_LIB_TYPE:=STATIC
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on

include C:/OpenCV-android-sdk/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk


LOCAL_MODULE	:= TayoNDK
#LOCAL_SRC_FILES := main.cpp HandGesture.cpp myImage.cpp roi.cpp facedetect.cpp facerecognize.cpp
LOCAL_LDLIBS +=  -llog -ldl
include $(BUILD_SHARED_LIBRARY)
