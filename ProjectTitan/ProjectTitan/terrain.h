#pragma once

#include "define.h"

#define TERRAIN_SIZE 100
#define TERRAIN_QUAD_SIZE 1

class VertexBuffer;
class Camera;

class Terrain
{
public:
	Terrain();
	~Terrain();

	void Init(const INT program, const CHAR* path = 0);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void SetHeightMap(const CHAR* path);
	void SetSize(FLOAT x, FLOAT y, FLOAT z);
	void SetShadowCamera(Camera camera);
	void SetShadowMap(UINT shadowMap);
	void Draw();

	UINT mProgram;
	esm::vec3* mPosition;
	VertexBuffer* mVertices;

	struct
	{
		UCHAR* mHeightMapContent;
		INT mHeightMapWidth, mHeightMapHeight;
	};

	esm::mat4* mModelMatrix;
	FLOAT *mLightViewMatrix, *mLightProjectMatrix;

private:
	FLOAT _getHeight(INT x, INT y);
	esm::vec3 _getNormal(INT x, INT y);
};
