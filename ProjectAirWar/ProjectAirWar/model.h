#ifndef __MODEL_H
#define __MODEL_H

#include "define.h"

//#define MODEL_MAX_TIME 3.0f

class Model;

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
	static void ReleaseVertexBufferObject(UINT vbo);

	VertexBuffer();
	~VertexBuffer();

	void Release();
	void SetSize(INT vertexesCount);
	void SetPosition(INT index, FLOAT x, FLOAT y, FLOAT z, FLOAT w = 1.0f);
	void SetColor(INT index, FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void SetTexcoord(INT index, FLOAT x, FLOAT y);
	void SetNormal(INT index, FLOAT x, FLOAT y, FLOAT z);
	void Bind();
	void UpdateData();
	void Unbind();
	void Clear();
	UINT GetVBO();
	Vertex* Get(INT index);

public:
	Vertex* mVertexes = NULL;
	INT mVertexesCount;
	UINT mVBO;
};


class Camera;

class Model
{
public:
	Model();
	virtual void Init(const INT program, const CHAR* modelPath);
	virtual void InitVAO();
	virtual void Draw();
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void SetMoveAdd(FLOAT x, FLOAT y, FLOAT z);
	void Scale(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	esm::vec3 GetPosition();
	void Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z);
	void MultiplyModelMatrix(esm::mat4 m);
	void LoadIdentity();
	void SetShadowCamera(Camera camera);
	Model * Clone();
	void Hide();
	void CloneRelease();
	// void MultiplyModelMatrix(FLOAT* m);

public:
	VertexBuffer * mVertexBuffer;
	esm::mat4* mModelMatrix;
	esm::vec3 mMoveAdded;
	FLOAT *mLightViewMatrix, *mLightProjectMatrix;

	INT mProgram;
	UINT mVertexArray;
	FLOAT mTime;
	BOOL mIsHide;
};

class Program;
typedef void(_stdcall *ModelEmitCallback) (Model * model);

class ModelEmit
{
public:
	ModelEmit();
	~ModelEmit();

	void Init(const CHAR * vs, const CHAR * fs, Camera * camera, const CHAR* modelPath, esm::vec3 emitDir, esm::vec3 emitPos, FLOAT speed = 2.0f, FLOAT time = 3.0f);
	void Update(FLOAT second);
	void SetCallback(ModelEmitCallback func);
	void Erase(Model * model);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(esm::vec3 pos);
	void SetLightPosition(FLOAT x, FLOAT y, FLOAT z, BOOL specular = TRUE);
	void SetTexture(const CHAR * name, const CHAR * filePath);
	void SetTexture(const CHAR * name, UINT texture);
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void Scale(FLOAT x, FLOAT y, FLOAT z);
	void Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z);
	void Emit();
	void Draw();
	void ShadowDraw();
	Model * GetSampleModel() const;

	FLOAT mSpeed, mTime;
	Program * mProgram;
	esm::vec3 mEmitDir, mEmitPos;
	Model * mSampleModel;
	std::vector<Model*> mModels;
	ModelEmitCallback mCallback;
};

#endif
