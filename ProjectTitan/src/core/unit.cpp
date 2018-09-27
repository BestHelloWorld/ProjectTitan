#include "unit.h"

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

	mProgram->SetUniform4f(SHADER_MATERIAL_AMBIENT, 1.0f, 1.0f, 1.0f, 1.0f);

	mAmbientColor = cm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	mDisffuseColor = cm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	mSpecularColor = cm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
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
	mProgram->SetUniform4f(SHADER_UNIFORM_LIGHT_POSITION, x, y, z, 0.0f);

	UpdateMaterial(specular);
}

void Unit::SetAmbientMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mAmbientColor.r = r;
	mAmbientColor.g = g;
	mAmbientColor.b = b;
	mAmbientColor.a = a;
	UpdateMaterial(FALSE);
}

void Unit::SetDisffuseMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mDisffuseColor.r = r;
	mDisffuseColor.g = g;
	mDisffuseColor.b = b;
	mDisffuseColor.a = a;
	UpdateMaterial(FALSE);
}

void Unit::SetSpecularMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mSpecularColor.r = r;
	mSpecularColor.g = g;
	mSpecularColor.b = b;
	mSpecularColor.a = a;
	UpdateMaterial(TRUE);
}

void Unit::UpdateMaterial(BOOL specular)
{
	mProgram->SetUniform4f(SHADER_MATERIAL_AMBIENT, mAmbientColor.r, mAmbientColor.g, mAmbientColor.b, mAmbientColor.a);
	mProgram->SetUniform4f(SHADER_MATERIAL_DIFFUSE, mDisffuseColor.r, mDisffuseColor.g, mDisffuseColor.b, mDisffuseColor.a);
	if (specular)
		mProgram->SetUniform4f(SHADER_MATERIAL_SPECULAR, mSpecularColor.r, mSpecularColor.g, mSpecularColor.b, mSpecularColor.a);
	else
		mProgram->SetUniform4f(SHADER_MATERIAL_SPECULAR, 0.0f, 0.0f, 0.0f, 1.0f);
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

void Unit::Move(cm::vec3 pos)
{
	mModel->Move(pos.x, pos.y, pos.z);
}

void Unit::SetMoveAdd(FLOAT x, FLOAT y, FLOAT z)
{
	//mMoveAdded.x = x;
	//mMoveAdded.y = y;
	//mMoveAdded.z = z;
	mModel->SetMoveAdd(x, y, z);
}

void Unit::Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z)
{
	mModel->Rotate(angle, x, y, z);
}

void Unit::LoadIdentity()
{
	mModel->LoadIdentity();
}

void Unit::PushMatrix()
{
	mModel->PushMatrix();
}

void Unit::PopMatrix()
{
	mModel->PopMatrix();
}

cm::vec3 Unit::GetPosition()
{
	return mModel->GetPosition();
}

cm::vec3 Unit::GetViewdir()
{
	return cm::vec3();
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
		cm::vec3 eye = cam->GetEyePos();
		mProgram->SetUniform4f(SHADER_UNIFORM_CAMERA_POSITION, eye.x, eye.y, eye.z, 1.0f);
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

void Unit::SetFogLimit(FLOAT nNear, FLOAT nFar)
{
	mProgram->SetFogLimit(nNear, nFar);
}

void Unit::SetFogColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mProgram->SetFogColor(r, g, b, a);
}
