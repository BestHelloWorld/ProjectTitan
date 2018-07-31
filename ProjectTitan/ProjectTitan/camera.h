#ifndef __CAMERA_H
#define __CAMERA_H

#include "define.h"

#define SENSITIVE 12.0f

class Camera
{
public:
	void Init(esm::vec3 eye = esm::vec3(0.0f, 0.0f, 0.0f), esm::vec3 center = esm::vec3(0.0f, 0.0f, -1.0f), esm::vec3 up = esm::vec3(0.0f, 1.0f, 0.0f));
	void Move(FLOAT x, FLOAT y, FLOAT z = 0);
	void Rotate(FLOAT yaw, FLOAT pitch, FLOAT roll = 0);
	esm::vec4 NormalizeDevicePos(esm::vec4 worldPos);
	void CameraForward(FLOAT offset);
	FLOAT* GetViewMatrix();
	FLOAT* GetProjectionMatrix();
	void Switch3D(FLOAT fov, FLOAT aspect);
	void Switch2D(FLOAT width, FLOAT height);

private:
	esm::mat4 _getViewMatrix();
	esm::mat4 _getProjectionMatrix();

public:
	esm::vec3 mViewDir;
	esm::vec3 mRightDir;
	esm::vec3 mEye;
	esm::vec3 mCenter;
	esm::vec3 mUp;
	FLOAT mOffset;
	FLOAT mYaw;
	FLOAT mRoll;
	FLOAT mPitch;

	INT mSkyboxLoc;
	UINT mSkyboxTex;

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

#endif
