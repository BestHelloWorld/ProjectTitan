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
	static UINT CreateVertexBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void* data = 0);
	static void ReleaseVertexBufferObject(UINT vbo);

	VertexBuffer();
	~VertexBuffer();

	void Release();

	// INIT SIZE
	void SetSize(INT vertexesCount);

	// SET PROPERTIES
	void SetPosition(INT index, FLOAT x, FLOAT y, FLOAT z, FLOAT w = 1.0f);
	void SetColor(INT index, FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void SetTexcoord(INT index, FLOAT x, FLOAT y);
	void SetNormal(INT index, FLOAT x, FLOAT y, FLOAT z);

	// USE VERTEX BUFFER OBJECT
	void Bind();
	void Unbind();

	// UPDATE
	void UpdateData();
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
	virtual void Release();
	virtual void Draw();

	// MODEL MATRIX
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void SetMoveAdd(FLOAT x, FLOAT y, FLOAT z);
	void Scale(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	cm::vec3 GetPosition();
	void Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z);
	void MultiplyModelMatrix(cm::mat4 m);

	// RESET MODEL MATRIX
	void LoadIdentity();
	void PushMatrix();
	void PopMatrix();

	// SHADER
	void SetShadowCamera(Camera camera);

	// FAST COPY
	Model * Clone();
	void CloneRelease();

	void Hide();
	// void MultiplyModelMatrix(FLOAT* m);

public:
	VertexBuffer * mVertexBuffer;
	cm::mat4* mModelMatrix;
	cm::vec3 mMoveAdded;
	FLOAT *mLightViewMatrix, *mLightProjectMatrix;

	INT mProgram;
	UINT mVertexArray;
	FLOAT mTime;
	BOOL mIsHide;

	std::stack<cm::mat4*> * mModelStack;
};

class Program;
typedef void( *ModelEmitCallback) (Model * model);

class ModelEmit
{
public:
	ModelEmit();
	~ModelEmit();

	void Init(const CHAR * vs, const CHAR * fs, Camera * camera, const CHAR* modelPath, cm::vec3 emitDir, cm::vec3 emitPos, FLOAT speed = 2.0f, FLOAT time = 3.0f);
	void Update(FLOAT second);

	// CALLBACK FUNCTION
	void SetCallback(ModelEmitCallback func);

	// ERASE FROM VECTOR
	void Erase(Model * model);
	void Clear();

	// MODEL MATRIX
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(cm::vec3 pos);
	void SetEmitDirection(FLOAT x, FLOAT y, FLOAT z);
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void Scale(FLOAT x, FLOAT y, FLOAT z);
	void Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z);

	// LIGHT
	void SetLightPosition(FLOAT x, FLOAT y, FLOAT z, BOOL specular = TRUE);

	// TEXTURE
	void SetTexture(const CHAR * name, const CHAR * filePath);
	void SetTexture(const CHAR * name, UINT texture);

	// NEW MODEL
	void Emit();
	void Emit(FLOAT x, FLOAT y, FLOAT z);

	// DRAW
	void Draw();

	// SHADER
	void ShadowDraw();

	Model * GetSampleModel() const;

	FLOAT mSpeed, mTime;
	Program * mProgram;
	cm::vec3 mEmitDir, mEmitPos;
	Model * mSampleModel;
	std::vector<Model*> mModels;
	std::vector<cm::vec3*> mModelsDir;
	ModelEmitCallback mCallback;
};

#endif
