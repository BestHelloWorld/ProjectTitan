#ifndef __CAMERA_H
#define __CAMERA_H

#include "define.h"

#define SENSITIVE 12.0f


class Camera
{
public:
	void Init(cm::vec3 eye = cm::vec3(0.0f, 0.0f, 0.0f), cm::vec3 center = cm::vec3(0.0f, 0.0f, -1.0f), cm::vec3 up = cm::vec3(0.0f, 1.0f, 0.0f));
	void Init(const Camera & camera);
	void Release();

	// WARNING : UPDATE
	void Update();

	// WARNING : DEFAULT NOT SWITCH CAMERA
	void Switch3D(FLOAT fov, FLOAT aspect);
	void Switch2D(FLOAT width, FLOAT height);

	// POSITION
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(cm::vec3 pos);
	void Rotate(FLOAT yaw, FLOAT pitch, FLOAT roll = 0);
	cm::vec4 WorldToScreen(cm::vec3 pos);
	cm::vec4 NormalizeDevicePos(cm::vec4 worldPos);
	cm::vec3 GetEyePos();

	void SetFar(FLOAT n_far);
	void SetNear(FLOAT n_near);

	void PushMatrix();
	void PopMatrix();

	// THIRD PERSON PERSPECTIVE
	void CameraForward(FLOAT offset);

	// GET LAST UPDATE MATRIX
	FLOAT* GetViewMatrix();
	FLOAT* GetProjectionMatrix();

	// CALC CURRENT MATRIX
	cm::mat4 _getViewMatrix();
	cm::mat4 _getProjectionMatrix();

public:
	// CORE MEMBER
	cm::vec3 mEye;
	cm::vec3 mCenter;
	cm::vec3 mUp;
	FLOAT mOffset;
	FLOAT mYaw;
	FLOAT mRoll;
	FLOAT mPitch;

	FLOAT mNear;
	FLOAT mFar;

	cm::vec3 mViewDir;
	cm::vec3 mRightDir;
	cm::vec3 mTPos;
	cm::mat4 * mViewMat;
	cm::mat4 * mProjMat;

	INT mSkyboxLoc;
	UINT mSkyboxTex;
	BOOL mIsInvalidate;

	enum CAMERA_STATUS
	{
		PERSPECTIVE, ORTHO
	};
	CAMERA_STATUS mCurrentStatus = PERSPECTIVE;
	struct
	{
		FLOAT mPerspFov, mPerspAspect;
		FLOAT mOrthoWidth, mOrthoHeight;
	};

	std::stack<Camera *> * mPosStack;
};


class Program;
class VertexBuffer;

typedef struct tagSkyboxTextures
{
	INT front;
	INT back;
	INT left;
	INT right;
	INT top;
	INT bottom;
}SkyboxTextures;

class Skybox
{
public:
	void Init(const CHAR * vs, const CHAR * fs, Camera * camera, const CHAR * front, const CHAR * back, const CHAR * left, const CHAR * right, const CHAR * top, const CHAR * bottom);
	void Release();
	void Draw();

	// POSITION
	void SetSize(FLOAT size);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	

private:
	Program * mProgram;
	Camera * mCamera;
	VertexBuffer * mVBO;
	INT mTexture;
	FLOAT mSize;

	cm::vec4 mColor;
};

#endif
