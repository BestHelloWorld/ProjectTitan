#ifndef __SCENE_MANAGER_H
#define __SCENE_MANAGER_H

#include "define.h"

class Program;
class FrameBuffer;
class SceneManager;
class FullScreenQuad;

#define TRANSFORM_TRANSLATE		0x2301
#define TRANSFORM_CUBE_ROTATE	0x2302
#define TRANSITION_STYLE		TRANSFORM_CUBE_ROTATE

#define CAPTURE_COLOR			0x2401
#define CAPTURE_DEPTH			0x2402

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init();
	virtual void SetViewport(FLOAT width, FLOAT height);
	virtual void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y);
	virtual void OnKey(UINT event, UCHAR chr);
	virtual void Draw(FLOAT s);

	UINT GetDepthBuffer();

	void InitDOF(UINT texture);
	SceneManager * GetSceneManager();

	void _setSceneManager(SceneManager * sm);

public:
	UINT mDOFTexture;
	BOOL mIsOpenDOF = FALSE;
private:
	SceneManager * mCurrentSM;
};


#define TRANSITION_TIME 1.0f

typedef struct tagDOF
{
	FrameBuffer * mFbo;
	FullScreenQuad * mDOF_fullScreen;
}DOF;

class SceneManager
{
public:
	~SceneManager();

	static SceneManager * GetInstance();

	// SCENE DISPATCH
	void InitScenes();
	void SetViewport(FLOAT width, FLOAT height);
	void Draw(FLOAT s);
	void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y);
	void OnKey(UINT event, UCHAR chr);

	// SCENE MANAGE
	void AddScene(const CHAR * sceneName, Scene * scene);
	BOOL Erase(const CHAR * sceneName);
	void Next(const CHAR * sceneName);

	// WINDOWS WIDTH & HEIGHT
	FLOAT GetViewportWidth();
	FLOAT GetViewportHeight();

	// CURRENT DRAW FRAME BUFFER
	FrameBuffer * GetFrameBuffer();

	void SetScene(const CHAR * sceneName);
	Scene * GetScene(const CHAR * sceneName);

	UINT GetColorBuffer();
	UINT GetDepthBuffer();

	void * CaptureScene(UINT capture_buffer = CAPTURE_COLOR);

	void _initTransition();
	void _update(FLOAT elapse);

public:
	Scene * mCurrent;
	std::map<const CHAR *, Scene*> mScenes;

	Program * mFboProg;
	FullScreenQuad * mMainFullQuad, * mSubFullQuad;
	FrameBuffer * mMainFbo, * mSubFbo, * mBufFbo;
	FLOAT mViewportWidth, mViewportHeight;

	FLOAT mTransitionTime;
private:

	SceneManager();
	static SceneManager * mManager;
	static DOF * mDOF;
};


#endif