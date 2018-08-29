#ifndef __UT_TERRAIN_H
#define __UT_TERRAIN_H

#include "core/define.h"

class Terrain;
class Camera;

class utterrain
{
public:
	utterrain();
	~utterrain();

	void Init(const CHAR * vs, const CHAR * fs, Camera * camera, INT xcount, INT zcount, const CHAR* path = 0);
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void Draw();

	void Release();

	Terrain * mCenterTerrain;
	std::vector<Terrain*> * mTerrains;
};

#endif

