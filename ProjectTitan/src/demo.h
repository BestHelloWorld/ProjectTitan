#ifndef __DEMO_H
#define __DEMO_H

#include "core/define.h"
#include "core/scene.h"
#include "core/program.h"
#include "core/fullscreenquad.h"
#include "core/shadow.h"
#include "core/camera.h"
#include "core/textbitmap.h"
#include "core/terrain.h"
#include "core/model.h"
#include "core/unit.h"
#include "core/model.h"
#include "core/sprite.h"

#include "utaircraft.h"

class MainScene : public Scene
{
public:
	void Init();
	void SetViewport(FLOAT width, FLOAT height);
	void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y);
	void OnKey(UINT event, UCHAR chr);
	void EventUpdate(FLOAT s);
	void Move(FLOAT x, FLOAT y, FLOAT z);

	void Draw(FLOAT s);

	void Start();
	void Stop();

	BOOL bMoveForward = FALSE, bMoveBackward = FALSE, bMoveLeft = FALSE, bMoveRight = FALSE, bMoveUp = FALSE, bMoveDown = FALSE;
	BOOL bShowDepth = FALSE;
	FLOAT currentMS = 0;
	UINT FPS = 0;
	UINT framesPerSecond = 0;
	WCHAR * str = new WCHAR[128]{ 0 };

	FLOAT gWidth, gHeight;

	FullScreenQuad fsq;
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
	void Init();
	void SetViewport(FLOAT width, FLOAT height);
	void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y);
	void OnKey(UINT event, UCHAR chr);
	void Draw(FLOAT s);

	Camera mCam;
	Skybox skybox;

	ButtonManager bm;
};

class TimeScene : public Scene
{
public:
	void Init();
	void SetViewport(FLOAT width, FLOAT height);
	void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y);
	void OnKey(UINT event, UCHAR chr);
	void Draw(FLOAT s);

	Camera mCam;
	Skybox skybox;

	ButtonManager bm;
};

#endif