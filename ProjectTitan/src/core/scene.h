#ifndef __SCENE_MANAGER_H
#define __SCENE_MANAGER_H

#include "define.h"

class Program;
class FrameBuffer;
class SceneManager;
class FullScreenQuad;

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

	void InitDOF(const CHAR * mix_vs, const CHAR * mix_fs, INT width, INT height);
	SceneManager * GetSceneManager();

	void _setSceneManager(SceneManager * sm);

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

	Scene * GetScene(const CHAR * sceneName);

	UCHAR * CaptureScene();

	void _initTransition();
	void _update(FLOAT elapse);

public:
	Scene * mCurrent;
	std::map<const CHAR *, Scene*> mScenes;

	Program * mFboProg;
	FullScreenQuad * mMainFullQuad, * mSubFullQuad;
	FrameBuffer * mMainFbo, * mSubFbo;
	FLOAT mViewportWidth, mViewportHeight;

	FLOAT mTransitionTime;
private:

	SceneManager();
	static SceneManager * mManager;
	static DOF * mDOF;
};


#endif