#ifndef __MODEL_H
#define __MODEL_H

#include "define.h"

struct Vertex
{
	FLOAT Position[4];
	FLOAT Color[4];
	FLOAT Texcoord[4];
	FLOAT Normal[4];
};

class VertexBuffer
{
public:
	static UINT CreateVertexBufferObject(ENUM bufferType, GLsizeiptr size, ENUM usage, void* data = 0);

	void SetSize(INT vertexesCount);
	void SetPosition(INT index, FLOAT x, FLOAT y, FLOAT z, FLOAT w = 1.0f);
	void SetColor(INT index, FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void SetTexcoord(INT index, FLOAT x, FLOAT y);
	void SetNormal(INT index, FLOAT x, FLOAT y, FLOAT z);
	void Bind();
	void Unbind();
	Vertex* Get(INT index);

public:
	Vertex* mVertexes;
	INT mVertexesCount;
	UINT mVBO;
};


class Camera;

class Model
{
public:
	Model();
	void Init(const INT program, const CHAR* modelPath);
	void Draw();
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void SetSize(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void MultiplyModelMatrix(esm::mat4 m);
	void LoadIdentity();
	void SetShadowCamera(Camera camera);
	// void MultiplyModelMatrix(FLOAT* m);

public:
	VertexBuffer* mVertexBuffer;
	esm::mat4* mModelMatrix;
	FLOAT *mLightViewMatrix, *mLightProjectMatrix;

	INT mProgram;
};

#endif
