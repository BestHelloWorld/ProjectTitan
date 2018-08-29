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
	virtual void InitVAO();
	virtual void Draw();
	virtual void Release();

	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void SetHeightMap(const CHAR* path);
	void SetSize(FLOAT x, FLOAT y, FLOAT z);

	// LIGHT
	void SetLightPos(FLOAT x, FLOAT y, FLOAT z, FLOAT w = 0.0f, BOOL specular = TRUE);
	void SetAmbientMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void SetDisffuseMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void SetSpecularMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void UpdateMaterial(BOOL specular);

	// TEXTURE
	void SetTexture(const CHAR * name, const CHAR * texture);
	void SetTexture(const CHAR * name, UINT texture);

	Terrain * Clone();
	Program * GetProgram();

	// SHADOW
	void SetShadowCamera(Camera camera);
	void SetShadowMap(UINT shadowMap);
	virtual void ShadowDraw();

	// MODEL MATRIX
	void PushMatrix();
	void PopMatrix();

	// FOG
	void SetFogLimit(FLOAT nNear, FLOAT nFar);
	void SetFogColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);

	std::stack<cm::mat4*> * mModelStack;

	Program * mProgram;
	Camera * mCamera;
	cm::vec3 * mPosition;
	VertexBuffer * mVertices;
	UINT mVertexArray;

	struct
	{
		UCHAR * mHeightMapContent;
		INT mHeightMapWidth, mHeightMapHeight;
	};

	cm::mat4* mModelMatrix;
	FLOAT *mLightViewMatrix, *mLightProjectMatrix;

	cm::vec4 mAmbientColor;
	cm::vec4 mDisffuseColor;
	cm::vec4 mSpecularColor;

private:
	FLOAT _getHeight(INT x, INT y);
	cm::vec3 _getNormal(INT x, INT y);
	inline BOOL Check();
};


inline BOOL Terrain::Check()
{
	if ((!mProgram) || (!mCamera))
		return FALSE;
	return TRUE;
}

#endif