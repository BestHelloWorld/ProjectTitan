#include "utterrain.h"

#include "terrain.h"
#include "camera.h"



utterrain::utterrain()
{
	mTerrains = NULL;
}

utterrain::~utterrain()
{

}


void utterrain::Init(const CHAR * vs, const CHAR * fs, Camera * camera, INT xcount, INT zcount, const CHAR* path)
{
	mTerrains = new std::vector<Terrain*>;

	mCenterTerrain = new Terrain;
	mCenterTerrain->Init(vs, fs, camera, path);
	for (INT i = 0; i < xcount; ++i)
	{
		for (INT j = 0; j < zcount; ++j)
		{

		}
	}
}

void utterrain::Move(FLOAT x, FLOAT y, FLOAT z)
{

}

void utterrain::Draw()
{

}

void utterrain::Release()
{
	for (INT i = 0; i < mTerrains->size(); ++i)
	{
		delete (*mTerrains)[i];
	}
	delete mTerrains;
}
