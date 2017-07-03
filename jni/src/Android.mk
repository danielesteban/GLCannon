LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_IMAGE_PATH := ../SDL_image
GLM_PATH := ../glm
IMGUI_PATH := ../imgui

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/${SDL_IMAGE_PATH} \
	$(LOCAL_PATH)/${GLM_PATH} \
	$(LOCAL_PATH)/${IMGUI_PATH}

LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	main.cpp camera.cpp mesh.cpp model.cpp shader.cpp \
	meshes/rotatingcube.cpp \
	models/cube.cpp \
	shaders/standard.cpp \
	${IMGUI_PATH}/imgui.cpp ${IMGUI_PATH}/imgui_draw.cpp ${IMGUI_PATH}/imgui_impl_sdl_gles.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
