#ifndef __UNIT_H
#define __UNIT_H

#include "define.h"

class Camera;
class Program;
class FrameBuffer;
class Model;

class Unit
{
public:
	virtual void Init(const CHAR * vs, const CHAR * fs, const CHAR * obj, Camera * cam);
	virtual void Draw();

	// Model MATRIX
	void LoadIdentity();
	void PushMatrix();
	void PopMatrix();

	void SetMoveAdd(FLOAT x, FLOAT y, FLOAT z);
	cm::vec3 GetPosition();
	cm::vec3 GetViewdir();

	// LIGHT
	void SetLightPos(FLOAT x, FLOAT y, FLOAT z, FLOAT w = 0.0f, BOOL specular = TRUE);
	void SetAmbientMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void SetDisffuseMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void SetSpecularMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void UpdateMaterial(BOOL specular);

	// POSITION	virtual 
	virtual void Move(FLOAT x, FLOAT y, FLOAT z);
	virtual void Move(cm::vec3 pos);
	virtual void Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z);
	virtual void Scale(FLOAT x, FLOAT y, FLOAT z);

	// TEXTURE
	void SetTexture(const CHAR * name, const CHAR * texture);
	void SetTexture(const CHAR * name, UINT texture);

	// FOG
	void SetFogLimit(FLOAT nNear, FLOAT nFar);
	void SetFogColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0);

	// SHADOW
	void ShadowInit(Camera * shadowCam, UINT shadowMap);
	virtual void ShadowDraw();


	cm::vec3 mMoveAdded;
	Program * mProgram = NULL;
	Model * mModel = NULL;

	BOOL mIsLight = FALSE;

	typedef struct tagShadowInfo
	{
		UINT shadowMap;
		Camera * shadowCam;
	}ShadowInfo;
	ShadowInfo * mShadow = NULL;

	cm::vec4 mAmbientColor;
	cm::vec4 mDisffuseColor;
	cm::vec4 mSpecularColor;

	cm::vec3 mViewdir;
	cm::vec3 mScale;
};


#endif