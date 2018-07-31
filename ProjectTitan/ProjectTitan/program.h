#ifndef __SHADER_H
#define __SHADER_H

#include "define.h"


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
	static UINT CompileShader(ENUM shaderType, const CHAR* shaderCode);
	static UINT CreateProgram(UINT vsShader, UINT fsShader);

public:
	void Init(const CHAR* vsPath, const CHAR* fsPath, Camera* camera);
	//void Bind(Camera& camera);
	void Bind();
	void Unbind();
	void SetTexture(const CHAR* name, const CHAR* path);
	UINT SetTexture(const CHAR* name, UINT texture);
	UINT SetTextureCube(const CHAR* name, UINT texture);
	void SetVector4f(const CHAR* name, FLOAT x, FLOAT y, FLOAT z, FLOAT w);

	inline UINT GetProgramId();

public:
	Camera* mCamera;
	UINT mProgram;
	INT mModelMatrixLocation, mViewMatrixLocation, mProjectMatrixLocation;

	std::map<std::string, UniformTexture *> mUniformTextures;
	std::map<std::string, UniformTextureCube *> mUniformTextureCubes;
	std::map<std::string, UniformVector4f *> mUniformVectors;
};

inline UINT Program::GetProgramId()
{
	return mProgram;
}

#endif
