#include "camera.h"

void Camera::Init(esm::vec3 eye, esm::vec3 center, esm::vec3 up)
{
	mEye = eye;
	mCenter = center;
	mViewDir = mCenter - mEye;
	mUp = up;
	mRightDir = mViewDir.cross(mUp);
	mOffset = 0.0f;

	mYaw = 270.0f;
	mPitch = 0.0f;
}

void Camera::Move(FLOAT x, FLOAT y, FLOAT z)
{
	if (z)
	{
		mEye = mEye - (mViewDir * z);
	}

	if (y != 0.0f)
	{
		mEye = mEye + (mUp * y);
	}

	if (x != 0.0f)
	{
		mEye = mEye + (mRightDir * x);
	}
}

void Camera::Rotate(FLOAT yaw, FLOAT pitch, FLOAT roll)
{
	if (yaw || pitch)
	{
		mYaw += (yaw * SENSITIVE);
		mPitch += (pitch * SENSITIVE);

		if (mPitch > 89.0f)
			mPitch = 89.0f;
		else if (mPitch < -89.0f)
			mPitch = -89.0f;

		mViewDir.x = esm::cos(mYaw) * esm::cos(mPitch);
		mViewDir.y = esm::sin(mPitch);
		mViewDir.z = esm::sin(mYaw) * esm::cos(mPitch);
		mViewDir = (mViewDir).normalize();

		//esm::quat q = esm::quat::euler2quat(mPitch, -mYaw, roll);
		//esm::mat3 m = esm::quat::quat2mat3(q);
		//mViewDir = m * mViewDir;
		//mViewDir = mViewDir.normalize();

		mRightDir = mViewDir.cross(mUp);

		//LOG_D("mViewDir, %f, %f, %f", mViewDir.x, mViewDir.y, mViewDir.z);
		//LOG_D("mRightDir, %f, %f, %f", mRightDir.x, mRightDir.y, mRightDir.z);
	}
}

esm::vec4 Camera::NormalizeDevicePos(esm::vec4 worldPos)
{
	//std::cout << esm::mat4(GetProjectionMatrix()).dump() << std::endl;
	esm::vec4 normalizePos = esm::mat4(GetProjectionMatrix())*esm::mat4(GetViewMatrix())*worldPos;
	return normalizePos / normalizePos.w;
}

void Camera::CameraForward(FLOAT offset)
{
	mOffset += offset;
}

FLOAT* Camera::GetViewMatrix()
{
	return (new esm::mat4(esm::lookAt(mEye - mOffset, mEye + mViewDir, mUp)))->v;
}

FLOAT* Camera::GetProjectionMatrix()
{
	switch (mCurrentStatus)
	{
	case Camera::PERSPECTIVE:
	{
		return (new esm::mat4(esm::perspective(mPerspFov, mPerspAspect, 0.1f, 100.0f)))->v;
	}
	case Camera::ORTHO:
	{
		FLOAT halfWidth = mOrthoWidth / 2;
		FLOAT halfHeight = mOrthoHeight / 2;
		return (new esm::mat4(esm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 100.0f)))->v;
	}
	default:
		break;
	}

	return NULL;
}

esm::mat4 Camera::_getViewMatrix()
{
	return esm::lookAt(mEye - mOffset, mEye + mViewDir, mUp);
}

esm::mat4 Camera::_getProjectionMatrix()
{
	switch (mCurrentStatus)
	{
	case Camera::PERSPECTIVE:
	{
		return esm::perspective(mPerspFov, mPerspAspect, 0.1f, 100.0f);
	}
	case Camera::ORTHO:
	{
		FLOAT halfWidth = mOrthoWidth / 2;
		FLOAT halfHeight = mOrthoHeight / 2;
		return esm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 100.0f);
	}
	default:
		break;
	}

	return esm::mat4(1.0f);
}

void Camera::Switch3D(FLOAT fov, FLOAT aspect)
{
	mCurrentStatus = PERSPECTIVE;
	mPerspFov = fov, mPerspAspect = aspect;
}

void Camera::Switch2D(FLOAT width, FLOAT height)
{
	mCurrentStatus = ORTHO;
	mOrthoWidth = width, mOrthoHeight = height;
}