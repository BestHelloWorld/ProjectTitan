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
#include <stack>
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
#define LOG_D(...)		__android_log_print(ANDROID_LOG_DEBUG, "NDK-debug", __VA_ARGS__)
#define LOG_E(...)		__android_log_print(ANDROID_LOG_ERROR, "NDK-error", __VA_ARGS__)

#elif _WIN32
#define LOG_D(...)		printf(__VA_ARGS__); printf("\n")
#define LOG_E(...)		printf(__VA_ARGS__); printf("\n")
#endif

#define LWORD(i)		i & 0xFFFF
#define HWORD(i)		i >> 16 & 0xFFFF

#define RAD(x)			x*3.1415926f/180.0f
#define POW_2(x)		pow(x, 2)
#define POW_4(x)		pow(x, 4)
#define POW_8(x)		pow(x, 8)

#define MS_TO_S(ms)		ms / 1000.0f
#define S_TO_MS(s)		s * 1000.0f

#define NEGATIVE_ONE	-1
#define ONE				1
#define NULL			0

#define BOOL			bool
#define ULONG			unsigned long
#define UINT			unsigned int
#define INT				int
#define FLOAT			float
#define CHAR			char
#define UCHAR			unsigned char
#define SHORT			short
#define USHORT			unsigned short
#define WCHAR			wchar_t

#define TRUE			1
#define FALSE			0

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

#define SHADER_UNIFORM_LIGHT_POSITION "U_LightPos"
#define SHADER_UNIFORM_CAMERA_POSITION "U_CameraPos"
#define SHADER_UNIFORM_COLOR "U_Color"

#define SHADER_SAMPLER2D_MAIN_TEXTURE "U_Texture"
#define SHADER_SAMPLER2D_SHADOW_TEXTURE "U_ShadowMap"

#define SHADER_MATERIAL_AMBIENT "U_AmbientMaterial"
#define SHADER_MATERIAL_DIFFUSE "U_DiffuseMaterial"
#define SHADER_MATERIAL_SPECULAR "U_SpecularMaterial"

#define GL_CLEAR(r, g, b)	glClearColor(r, g, b, 1.0f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
#define GL_BLEND_BEGIN		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
#define GL_BLEND_END		glDisable(GL_BLEND)
#define GL_DEPTH_TEST_BEGIN glEnable(GL_DEPTH_TEST)
#define GL_DEPTH_TEST_END	glDisable(GL_DEPTH_TEST)


#define EVENT_UP			0x1101
#define EVENT_MOVE			0x1102
#define EVENT_DOWN			0x1103

//typedef void(_stdcall*DrawProc) (FLOAT ms);

// CAMERA CONFIG
#define MOVE_RATIO 6.0f

namespace
{

	typedef struct Point
	{
		INT x, y;
	};

	typedef struct Pointf
	{
		FLOAT x, y;
	};

	typedef struct Rect
	{
		INT left, top, right, bottom;
	};

	typedef struct Rectf
	{
		FLOAT left, top, right, bottom;
	};
}
#endif
