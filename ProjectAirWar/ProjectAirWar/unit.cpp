#include "Unit.h"

#include "program.h"
#include "model.h"
#include "camera.h"
#include "framebuffer.h"


void Unit::Init(const CHAR* vs, const CHAR* fs, const CHAR* obj, Camera* cam)
{
	mProgram = new Program;
	mModel = new Model;

	mProgram->Init(vs, fs, cam);
	mModel->Init(mProgram->GetProgramId(), obj);

	mProgram->SetVector4f(SHADER_MATERIAL_AMBIENT, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Unit::SetTexture(const CHAR* name, const CHAR* texture)
{
	mProgram->SetTexture(name, texture);
}

void Unit::SetTexture(const CHAR* name, UINT texture)
{
	mProgram->SetTexture(name, texture);
}

void Unit::SetLightPos(FLOAT x, FLOAT y, FLOAT z, FLOAT w, BOOL specular)
{
	mIsLight = TRUE;
	mProgram->SetVector4f(SHADER_MATERIAL_AMBIENT, 0.1f, 0.1f, 0.1f, 1.0f);
	mProgram->SetVector4f(SHADER_MATERIAL_DIFFUSE, 0.5f, 0.5f, 0.5f, 1.0f);
	if (specular)
		mProgram->SetVector4f(SHADER_MATERIAL_SPECULAR, 1.0f, 1.0f, 1.0f, 1.0f);
	else
		mProgram->SetVector4f(SHADER_MATERIAL_SPECULAR, 0.0f, 0.0f, 0.0f, 1.0f);
	mProgram->SetVector4f(SHADER_UNIFORM_LIGHT_POSITION, x, y, z, 0.0f);
}

void Unit::ShadowInit(Camera * shadowCam, UINT shadowMap)
{
	mShadow = new ShadowInfo;
	mShadow->shadowMap = shadowMap;
	mShadow->shadowCam = shadowCam;
}

void Unit::ShadowDraw()
{
	if (!mShadow)
		return;

	mModel->Draw();
}

void Unit::Move(FLOAT x, FLOAT y, FLOAT z)
{
	mModel->Move(x, y, z);
}

void Unit::Move(esm::vec3 pos)
{
	mModel->Move(pos.x, pos.y, pos.z);
}

void Unit::SetMoveAdd(FLOAT x, FLOAT y, FLOAT z)
{
	mMoveAdded.x = x;
	mMoveAdded.y = y;
	mMoveAdded.z = z;
}

void Unit::Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z)
{
	mModel->Rotate(angle, x, y, z);
}

void Unit::LoadIdentity()
{
	mModel->LoadIdentity();
}

esm::vec3 Unit::GetPosition()
{
	return mModel->GetPosition();
}

void Unit::Scale(FLOAT x, FLOAT y, FLOAT z)
{
	mModel->Scale(x, y, z);
}

void Unit::Draw()
{
	if (mIsLight)
	{
		Camera* cam = mProgram->GetCamera();
		mProgram->SetVector4f(SHADER_UNIFORM_CAMERA_POSITION, cam->mEye.x, cam->mEye.y, cam->mEye.z, 1.0f);
	}
	if (mShadow)
	{
		mModel->SetShadowCamera(*mShadow->shadowCam);
		mProgram->SetTexture(SHADER_SAMPLER2D_SHADOW_TEXTURE, mShadow->shadowMap);
	}

	mProgram->Bind();
	mModel->Draw();
	mProgram->Unbind();
}
