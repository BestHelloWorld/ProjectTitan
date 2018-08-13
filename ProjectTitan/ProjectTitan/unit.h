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
	void SetTexture(const CHAR * name, const CHAR * texture);
	void SetTexture(const CHAR * name, UINT texture);
	void LoadIdentity();
	void SetLightPos(FLOAT x, FLOAT y, FLOAT z, FLOAT w = 0.0f, BOOL specular = TRUE);
	virtual void Move(FLOAT x, FLOAT y, FLOAT z);
	virtual void Move(cm::vec3 pos);
	void SetMoveAdd(FLOAT x, FLOAT y, FLOAT z);
	cm::vec3 GetPosition();
	void Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z);
	void Scale(FLOAT x, FLOAT y, FLOAT z);
	virtual void Draw();

	void ShadowInit(Camera * shadowCam, UINT shadowMap);
	virtual void ShadowDraw();


	cm::vec3 mMoveAdded;
	Program * mProgram = NULL;
	Model * mModel = NULL;

	BOOL mIsLight = FALSE;

	typedef struct ShadowInfo
	{
		UINT shadowMap;
		Camera * shadowCam;
	};
	ShadowInfo * mShadow = NULL;
};


#endif