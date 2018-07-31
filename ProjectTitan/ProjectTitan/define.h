#ifndef __GGL_H
#define __GGL_H


#ifdef __ANDROID__
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#elif _WIN32
//if use glew static library, define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

#include <functional>
#include <list>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>

#include "esm.hpp"


#define ENUM GLenum

#ifdef __ANDROID__
#define LOG_D(...) __android_log_print(ANDROID_LOG_DEBUG, "NDK-debug", __VA_ARGS__)
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, "NDK-error", __VA_ARGS__)

#elif _WIN32
#define LOG_D(...) printf(__VA_ARGS__); printf("\n")
#define LOG_E(...) printf(__VA_ARGS__); printf("\n")
#endif

#define RAD(x) x *3.1415926f/180.0f
#define NEGATIVE_ONE -1
#define ONE 1
#define NULL 0

#define BOOL bool
#define ULONG unsigned long
#define UINT unsigned int
#define INT int
#define FLOAT float
#define CHAR char
#define UCHAR unsigned char
#define SHORT short
#define USHORT unsigned short
#define WCHAR wchar_t

#define TRUE 1
#define FALSE 0

#define SHADER_ATTRIB_POSITION "position"
#define SHADER_ATTRIB_TEXCOORD "texcoord"
#define SHADER_ATTRIB_NORMAL "normal"
#define SHADER_ATTRIB_COLOR "color"
#define SHADER_UNIFORM_MODEL_MATRIX "ModelMatrix"
#define SHADER_UNIFORM_VIEW_MATRIX "ViewMatrix"
#define SHADER_UNIFORM_PROJECTION_MATRIX "ProjectMatrix"

#define SHADER_UNIFORM_NORMAL_MATRIX "NormalMatrix"
#define SHADER_UNIFORM_LIGHT_VIEW_MATRIX "LightViewMatrix"
#define SHADER_UNIFORM_LIGHT_PROJECTION_MATRIX "LightProjectMatrix"

#define SHADER_SAMPLER2D_MAIN_TEXTURE "U_Texture"
#define SHADER_SAMPLER2D_SHADOW_TEXTURE "U_ShadowMap"


// CAMERA CONFIG
#define MOVE_RATIO 0.05f

namespace
{

	typedef struct POINT
	{
		INT x, y;
	}*LPPOINT;

	typedef struct POINTF
	{
		FLOAT x, y;
	}*LPPOINTF;

	typedef struct RECT
	{
		INT left, top, right, bottom;
	}*LPRECT;

	typedef struct RECTF
	{
		FLOAT left, top, right, bottom;
	}*LPRECTF;

}
#endif
