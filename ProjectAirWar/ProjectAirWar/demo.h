#ifndef __DEMO_H
#define __DEMO_H

#include "define.h"

#include "scene.h"

#include "program.h"
#include "fullscreenquad.h"
#include "shadow.h"
#include "utaircraft.h"
#include "camera.h"
#include "textbitmap.h"
#include "terrain.h"
#include "model.h"

class MainScene : public Scene
{
public:
	virtual void Init();
	virtual void SetViewport(FLOAT width, FLOAT height);
	virtual void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y);
	virtual void OnKey(UINT event, UCHAR chr);
	void EventUpdate(FLOAT s);
	void Move(FLOAT x, FLOAT y, FLOAT z);

	virtual void Draw(FLOAT s);

	BOOL bMoveForward = FALSE, bMoveBackward = FALSE, bMoveLeft = FALSE, bMoveRight = FALSE, bMoveUp = FALSE, bMoveDown = FALSE;
	BOOL bShowDepth = FALSE;
	FLOAT currentMS = 0;
	UINT FPS = 0;
	UINT framesPerSecond = 0;
	WCHAR * str = new WCHAR[128]{ 0 };

	FLOAT gWidth, gHeight;

	FullScreenQuad fsq;
	Program fsqProg;
	Shadow shadow;

	// CAMERA
	Camera cam, shadowCam, cam2d;

	Skybox skybox;

	// TEXT
	TextBitmap * tb;
};

class WelcScene : public Scene
{
public:
	virtual void Init();
	virtual void SetViewport(FLOAT width, FLOAT height);
	virtual void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y);
	virtual void OnKey(UINT event, UCHAR chr);
	virtual void Draw(FLOAT s);

	Camera mCam;
	Skybox skybox;


};

#endif