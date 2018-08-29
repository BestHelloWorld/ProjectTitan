#ifndef __SHADER_H
#define __SHADER_H

#include "define.h"

#define CHECK(x) if(!Check()) return x

struct UniformTexture
{
	INT location;
	UINT texture;

	UniformTexture()
	{
		location = -1;
		texture = 0;
	}
};

struct UniformTextureCube
{
	INT location;
	UINT texture;

	UniformTextureCube()
	{
		location = -1;
		texture = 0;
	}
};

struct UniformVector4f
{
	INT location;
	FLOAT v[4];

	UniformVector4f()
	{
		location = -1;
		memset(v, 0, sizeof(FLOAT) * 4);
	}
};

class Camera;

class Program
{
public:
	static UINT CompileShader(GLenum shaderType, const CHAR* shaderCode);
	static UINT CreateProgram(UINT vsShader, UINT fsShader);

public:
	void Init(const CHAR* vsPath, const CHAR* fsPath, Camera* camera);
	void Release();
	void SetCamera(Camera * camera);
	void Bind();
	void Unbind();
	Camera * GetCamera();
	UINT SetTexture(const CHAR* name, const CHAR* path);
	UINT SetTexture(const CHAR* name, UINT texture);
	UINT SetTextureCube(const CHAR* name, UINT texture);
	void SetVector4f(const CHAR* name, FLOAT x, FLOAT y, FLOAT z, FLOAT w);

	void SetFogLimit(FLOAT nNear, FLOAT nFar);
	void SetFogColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);

	inline UINT GetProgramId();

public:
	Camera* mCamera;
	UINT mProgram;
	INT mModelMatrixLocation, mViewMatrixLocation, mProjectMatrixLocation;

	std::map<std::string, UniformTexture *> mUniformTextures;
	std::map<std::string, UniformTextureCube *> mUniformTextureCubes;
	std::map<std::string, UniformVector4f *> mUniformVectors;

	cm::vec4 mFogColor;
	cm::vec2 mFogLimit;

	UINT mVertShader, mFragShader;
};

inline UINT Program::GetProgramId()
{
	return mProgram;
}

#endif
