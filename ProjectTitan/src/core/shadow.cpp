#include "shadow.h"

#include "unit.h"
#include "terrain.h"
#include "framebuffer.h"
#include "camera.h"
#include "program.h"
#include "model.h"


void Shadow::Init(const CHAR * vs, const CHAR * fs, Camera * shadowCamera, INT fbo_width, INT fbo_height)
{
	mFbo = new FrameBuffer;
	mProgram = new Program;

	mCamera = shadowCamera;

	mProgram->Init(vs, fs, shadowCamera);
	mFbo->Init();
	mFbo->AttachColorBuffer(FBO_COLOR, GL_COLOR_ATTACHMENT0, fbo_width, fbo_height);
	mFbo->AttachDepthBuffer(FBO_DEPTH, fbo_width, fbo_height);
	mFbo->AttachFinish();
}
void Shadow::Add(Unit * unit)
{
	if (unit == NULL)
		return;

	mDrawList.push_back(unit);
	unit->ShadowInit(mCamera, mFbo->GetBuffer(FBO_DEPTH));
}

void Shadow::Add(Terrain * terrain)
{
	if (terrain == NULL)
		return;

	mDrawTerrainList.push_back(terrain);
	terrain->SetShadowCamera(*mCamera);
	//terrain->GetProgram()->SetTexture(SHADER_SAMPLER2D_SHADOW_TEXTURE, mFbo->GetBuffer(FBO_DEPTH));
	terrain->SetShadowMap(mFbo->GetBuffer(FBO_DEPTH));
}

void Shadow::Add(ModelEmit * emit)
{
	mDrawModelEmitList.push_back(emit);
}

void Shadow::Draw()
{
	mFbo->Bind();

	CLEAR_COLOR(0.0f, 0.0f, 0.0f);
	mProgram->Bind();

	for(UINT i = 0; i < mDrawList.size(); ++i)
	{
		mDrawList[i]->ShadowDraw();
	}

	for(UINT i = 0; i < mDrawTerrainList.size(); ++i)
	{
		mDrawTerrainList[i]->ShadowDraw();
	}

	for (UINT i = 0; i < mDrawModelEmitList.size(); ++i)
	{
		mDrawModelEmitList[i]->ShadowDraw();
	}

	mProgram->Unbind();
	mFbo->Unbind();
}

UINT Shadow::GetColorBuffer()
{
	return mFbo->GetBuffer(FBO_COLOR);
}

UINT Shadow::GetShadowMap()
{
	return mFbo->GetBuffer(FBO_DEPTH);
}

FrameBuffer * Shadow::GetFrameBuffer()
{
	return mFbo;
}