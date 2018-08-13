#include "camera.h"

#include "program.h"
#include "model.h"
#include "utils.h"



void Camera::Init(esm::vec3 eye, esm::vec3 center, esm::vec3 up)
{
	mEye = eye;
	mCenter = center;
	mViewDir = mCenter - mEye;
	mUp = up;
	mRightDir = mViewDir.cross(mUp);
	mOffset = 0.0f;
	mTPos = esm::vec3(0.0f);

	mYaw = 270.0f;
	mPitch = 0.0f;

	mViewMat = new esm::mat4(1.0f);
	mProjMat = new esm::mat4(1.0f);

}

void Camera::Move(FLOAT x, FLOAT y, FLOAT z)
{
	mTPos.z += z;
	mTPos.y += y;
	mTPos.x += x;
}

void Camera::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	mEye.x = x;
	mEye.y = y;
	mEye.z = z;
}

void Camera::SetPosition(esm::vec3 pos)
{
	SetPosition(pos.x, pos.y, pos.z);
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
	}
}

void Camera::Update()
{
	// UPDATE RAY
	if (mYaw && mPitch)
	{
		mViewDir.x = esm::cos(mYaw) * esm::cos(mPitch);
		mViewDir.y = esm::sin(mPitch);
		mViewDir.z = esm::sin(mYaw) * esm::cos(mPitch);
		mViewDir = mViewDir.normalize();
		mRightDir = mViewDir.cross(mUp);
	}

	// UPDATE POSITION
	mEye = mEye + (mRightDir * mTPos.x);
	mEye = mEye + (mUp * mTPos.y);
	mEye = mEye - (mViewDir * mTPos.z);
	mTPos = esm::vec3(0.0f);

	*mViewMat = _getViewMatrix();
	//*mProjMat = _getProjectionMatrix();
}

esm::vec4 Camera::WorldToScreen(esm::vec3 pos)
{
	esm::vec4 r = _getProjectionMatrix()*_getViewMatrix() * esm::vec4(pos);
	return r;
}

esm::vec4 Camera::NormalizeDevicePos(esm::vec4 worldPos)
{
	//std::cout << esm::mat4(GetProjectionMatrix()).dump() << std::endl;
	esm::vec4 normalizePos = _getProjectionMatrix()*_getViewMatrix()*worldPos;
	return normalizePos / normalizePos.w;
}

void Camera::CameraForward(FLOAT offset)
{
	mOffset -= offset;
}

esm::vec3 Camera::GetEyePos()
{
	return (mEye - (mViewDir * mOffset));
}

FLOAT* Camera::GetViewMatrix()
{
	//LOG_D("%s", mViewMat->dump());
	return mViewMat->v;
}

FLOAT* Camera::GetProjectionMatrix()
{
	//LOG_D("%s", mProjMat->dump());
	return mProjMat->v;
}

esm::mat4 Camera::_getViewMatrix()
{
	return esm::lookAt(mEye - (mViewDir * mOffset), mEye + mViewDir, mUp);
}

esm::mat4 Camera::_getProjectionMatrix()
{
	switch (mCurrentStatus)
	{
	case Camera::PERSPECTIVE:
	{
		return esm::perspective(mPerspFov, mPerspAspect, 0.1f, 100.0f).v;
	}
	case Camera::ORTHO:
	{
		FLOAT halfWidth = mOrthoWidth / 2;
		FLOAT halfHeight = mOrthoHeight / 2;
		return esm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 100.0f).v;
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

	*mViewMat = _getViewMatrix();
	*mProjMat = _getProjectionMatrix();
}

void Camera::Switch2D(FLOAT width, FLOAT height)
{
	mCurrentStatus = ORTHO;
	mOrthoWidth = width, mOrthoHeight = height;

	*mViewMat = _getViewMatrix();
	*mProjMat = _getProjectionMatrix();
}


void Skybox::Init(const CHAR * vs, const CHAR * fs, Camera * camera, const CHAR * front, const CHAR * back, const CHAR * left, const CHAR * right, const CHAR * top, const CHAR * bottom)
{
	mProgram = new Program;
	mProgram->Init(vs, fs, camera);
	mProgram->SetVector4f(SHADER_MATERIAL_AMBIENT, 1.0f, 1.0f, 1.0f, 1.0f);

	mCamera = camera;

	mTexture = CreateTextureCubeFromBMP(front, back, left, right, top, bottom);

	mProgram->SetTextureCube(SHADER_SAMPLER2D_MAIN_TEXTURE, mTexture);

	mVBO = new VertexBuffer;
	mVBO->SetSize(36);

	// FRONT
	INT index = 0;
	mVBO->SetPosition(index + 0, -1.0f, 1.0f, -1.0f);
	mVBO->SetPosition(index + 1, -1.0f, -1.0f, -1.0f);
	mVBO->SetPosition(index + 2, 1.0f, -1.0f, -1.0f);
	mVBO->SetPosition(index + 3, -1.0f, 1.0f, -1.0f);
	mVBO->SetPosition(index + 4, 1.0f, -1.0f, -1.0f);
	mVBO->SetPosition(index + 5, 1.0f, 1.0f, -1.0f);

	// BACK
	index += 6;
	mVBO->SetPosition(index + 0, 1.0f, 1.0f, 1.0f);
	mVBO->SetPosition(index + 1, 1.0f, -1.0f, 1.0f);
	mVBO->SetPosition(index + 2, -1.0f, -1.0f, 1.0f);
	mVBO->SetPosition(index + 3, 1.0f, 1.0f, 1.0f);
	mVBO->SetPosition(index + 4, -1.0f, -1.0f, 1.0f);
	mVBO->SetPosition(index + 5, -1.0f, 1.0f, 1.0f);

	// LEFT
	index += 6;
	mVBO->SetPosition(index + 0, -1.0f, 1.0f, 1.0f);
	mVBO->SetPosition(index + 1, -1.0f, -1.0f, 1.0f);
	mVBO->SetPosition(index + 2, -1.0f, -1.0f, -1.0f);
	mVBO->SetPosition(index + 3, -1.0f, 1.0f, 1.0f);
	mVBO->SetPosition(index + 4, -1.0f, -1.0f, -1.0f);
	mVBO->SetPosition(index + 5, -1.0f, 1.0f, -1.0f);

	// RIGHT
	index += 6;
	mVBO->SetPosition(index + 0, 1.0f, 1.0f, -1.0f);
	mVBO->SetPosition(index + 1, 1.0f, -1.0f, -1.0f);
	mVBO->SetPosition(index + 2, 1.0f, -1.0f, 1.0f);
	mVBO->SetPosition(index + 3, 1.0f, 1.0f, -1.0f);
	mVBO->SetPosition(index + 4, 1.0f, -1.0f, 1.0f);
	mVBO->SetPosition(index + 5, 1.0f, 1.0f, 1.0f);

	// TOP
	index += 6;
	mVBO->SetPosition(index + 0, -1.0f, 1.0f, 1.0f);
	mVBO->SetPosition(index + 1, -1.0f, 1.0f, -1.0f);
	mVBO->SetPosition(index + 2, 1.0f, 1.0f, -1.0f);
	mVBO->SetPosition(index + 3, -1.0f, 1.0f, 1.0f);
	mVBO->SetPosition(index + 4, 1.0f, 1.0f, -1.0f);
	mVBO->SetPosition(index + 5, 1.0f, 1.0f, 1.0f);

	// BOTTOM
	index += 6;
	mVBO->SetPosition(index + 0, -1.0f, -1.0f, -1.0f);
	mVBO->SetPosition(index + 1, -1.0f, -1.0f, 1.0f);
	mVBO->SetPosition(index + 2, 1.0f, -1.0f, 1.0f);
	mVBO->SetPosition(index + 3, -1.0f, -1.0f, -1.0f);
	mVBO->SetPosition(index + 4, 1.0f, -1.0f, 1.0f);
	mVBO->SetPosition(index + 5, 1.0f, -1.0f, -1.0f);

	mVBO->UpdateData();
	mVBO->Clear();

	mSize = 1.0f;
}

void Skybox::Release()
{

}

void Skybox::SetSize(FLOAT size)
{
	mSize = size;
}

void Skybox::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	mCamera->SetPosition(x, y, z);
}

void Skybox::Draw()
{
	mProgram->Bind();
	mVBO->Bind();
	INT location = 0;
	location = glGetAttribLocation(mProgram->GetProgramId(), SHADER_ATTRIB_POSITION);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((Vertex*)0)->Position);
	}

	mVBO->Unbind();

	location = glGetUniformLocation(mProgram->GetProgramId(), SHADER_UNIFORM_MODEL_MATRIX);
	if (location >= 0)
	{
		esm::vec4 pos = mCamera->GetEyePos();
		//esm::vec4 pos = mCamera->mEye;
		if (mSize == 1.0f)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, esm::translate(pos.x, pos.y, pos.z).v);
		}
		else
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, (esm::translate(pos.x, pos.y, pos.z) * esm::scale(mSize, mSize, mSize)).v);
		}
	}

	location = glGetUniformLocation(mProgram->GetProgramId(), SHADER_UNIFORM_CAMERA_POSITION);
	if (location >= 0 && mCamera)
	{
		glUniform4fv(location, 1, mCamera->mViewDir.v);
	}

	glDrawArrays(GL_TRIANGLES, 0, 36);

	mProgram->Unbind();
}
