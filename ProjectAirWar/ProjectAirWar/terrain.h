#ifndef __TERRAIN_H
#define __TERRAIN_H

#include "define.h"

#define TERRAIN_SIZE 200
#define TERRAIN_QUAD_SIZE 1


class VertexBuffer;
class Camera;
class Program;

class Terrain
{
public:
	Terrain();
	~Terrain();

	virtual void Init(const CHAR * vs, const CHAR * fs, Camera * camera, const CHAR* path = 0);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void SetHeightMap(const CHAR* path);
	void SetSize(FLOAT x, FLOAT y, FLOAT z);
	void SetLightPos(FLOAT x, FLOAT y, FLOAT z, FLOAT w = 0.0f, BOOL specular = TRUE);
	void SetTexture(const CHAR * name, const CHAR * texture);
	void SetTexture(const CHAR * name, UINT texture);
	Terrain * Clone();
	Program * GetProgram();
	virtual void InitVAO();
	virtual void Draw();

	void SetShadowCamera(Camera camera);
	void SetShadowMap(UINT shadowMap);
	virtual void ShadowDraw();

	void PushMatrix();
	void PopMatrix();
	std::stack<esm::mat4*> mStack;

	Program * mProgram;
	Camera * mCamera;
	esm::vec3 * mPosition;
	VertexBuffer * mVertices;
	UINT mVertexArray;

	struct
	{
		UCHAR * mHeightMapContent;
		INT mHeightMapWidth, mHeightMapHeight;
	};

	esm::mat4* mModelMatrix;
	FLOAT *mLightViewMatrix, *mLightProjectMatrix;

private:
	FLOAT _getHeight(INT x, INT y);
	esm::vec3 _getNormal(INT x, INT y);
	inline BOOL Check();
};


inline BOOL Terrain::Check()
{
	if ((!mProgram) || (!mCamera))
		return FALSE;
	return TRUE;
}

#endif