#ifndef __SHADOW_H
#define __SHADOW_H

#include "define.h"

class Unit;
class Terrain;
class Camera;
class Program;
class FrameBuffer;
class Model;
class ModelEmit;

class Shadow
{
public:
	void Init(const CHAR * vs, const CHAR * fs, Camera * shadowCamera, INT fbo_width, INT fbo_height);
	void Add(Unit * unit);
	void Add(Terrain * terrain);
	void Add(Model * model);
	void Add(ModelEmit * emit);
	void Clear();

	void Draw();

	void SetDrawFunction(void(*fun)());

	UINT GetColorBuffer();
	UINT GetDepthBuffer();
	FrameBuffer * GetFrameBuffer();

	std::vector<Unit*>		mDrawList;
	std::vector<Terrain*>	mDrawTerrainList;
	std::vector<Model*>		mDrawModelList;
	std::vector<ModelEmit*> mDrawModelEmitList;

	FrameBuffer * mFbo;
	Program * mProgram;
	Camera * mCamera;

	void(*mShadowDraw)() = NULL;

};


#endif