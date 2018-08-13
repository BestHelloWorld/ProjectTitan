#ifndef __SHADOW_H
#define __SHADOW_H

#include "define.h"


#define FBO_DEPTH "depth"
#define FBO_COLOR "color"

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
	void Init(const CHAR * vs, const CHAR * fs, Camera * shadowCamera, FLOAT fboWidth, FLOAT fboHeight);
	void Add(Unit * unit);
	void Add(Terrain * terrain);
	void Add(Model * model);
	void Add(ModelEmit * emit);
	void Draw();
	UINT GetColorBuffer();
	UINT GetShadowMap();
	FrameBuffer * GetFrameBuffer();

	std::vector<Unit*> mDrawList;
	std::vector<Terrain*> mDrawTerrainList;
	std::vector<Model*> mDrawModelList;
	std::vector<ModelEmit*> mDrawModelEmitList;

	FrameBuffer * mFbo;
	Program * mProgram;
	Camera * mCamera;

};


#endif