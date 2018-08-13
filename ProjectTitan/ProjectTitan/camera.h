#ifndef __CAMERA_H
#define __CAMERA_H

#include "define.h"

#define SENSITIVE 12.0f


class Camera
{
public:
	void Init(esm::vec3 eye = esm::vec3(0.0f, 0.0f, 0.0f), esm::vec3 center = esm::vec3(0.0f, 0.0f, -1.0f), esm::vec3 up = esm::vec3(0.0f, 1.0f, 0.0f));
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(esm::vec3 pos);
	void Rotate(FLOAT yaw, FLOAT pitch, FLOAT roll = 0);
	void Update();
	esm::vec4 WorldToScreen(esm::vec3 pos);
	esm::vec4 NormalizeDevicePos(esm::vec4 worldPos);
	void CameraForward(FLOAT offset);
	esm::vec3 GetEyePos();
	FLOAT* GetViewMatrix();
	FLOAT* GetProjectionMatrix();
	void Switch3D(FLOAT fov, FLOAT aspect);
	void Switch2D(FLOAT width, FLOAT height);

	esm::mat4 _getViewMatrix();
	esm::mat4 _getProjectionMatrix();

public:
	esm::vec3 mViewDir;
	esm::vec3 mRightDir;
	esm::vec3 mEye;
	esm::vec3 mCenter;
	esm::vec3 mUp;
	esm::vec3 mTPos;
	esm::mat4 * mViewMat;
	esm::mat4 * mProjMat;
	FLOAT mOffset;
	FLOAT mYaw;
	FLOAT mRoll;
	FLOAT mPitch;

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
	void SetSize(FLOAT size);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void Draw();

private:
	Program * mProgram;
	Camera * mCamera;
	VertexBuffer * mVBO;
	INT mTexture;
	INT mSize;
};

#endif
