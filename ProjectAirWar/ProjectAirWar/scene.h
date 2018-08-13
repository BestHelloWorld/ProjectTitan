#ifndef __SCENE_H
#define __SCENE_H

#include "define.h"


class Scene
{
public:
	virtual void Init() = 0;
	virtual void SetViewport(FLOAT width, FLOAT height) = 0;
	virtual void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y) = 0;
	virtual void OnKey(UINT event, UCHAR chr) = 0;
	virtual void Draw(FLOAT s) = 0;
};


class SceneManager
{
public:
	~SceneManager();

	static SceneManager * GetInstance();
	void InitScenes();
	void SetViewport(FLOAT width, FLOAT height);
	void AddScene(const CHAR * sceneName, Scene*scene);
	BOOL Erase(const CHAR * sceneName);
	void Next(const CHAR * sceneName);
	void Last();

	void Draw(FLOAT s);
	void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y);
	void OnKey(UINT event, UCHAR chr);


	Scene * mCurrent;
	typedef std::pair<std::string, Scene*> ScenePair;
	std::map<std::string, Scene*> mScenes;
private:

	SceneManager();
	static SceneManager * mManager;

};


#endif

