#ifndef __SCENE_MANAGER_H
#define __SCENE_MANAGER_H

#include "define.h"



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
};


class SceneManager
{
public:
	~SceneManager();

	static SceneManager * GetInstance();
	void InitScenes();
	void SetViewport(FLOAT width, FLOAT height);
	void AddScene(const CHAR * sceneName, Scene * scene);
	BOOL Erase(const CHAR * sceneName);
	void Next(const CHAR * sceneName);

	void Draw(FLOAT s);
	void OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y);
	void OnKey(UINT event, UCHAR chr);


	Scene * mCurrent;
	std::map<const CHAR *, Scene*> mScenes;
private:

	SceneManager();
	static SceneManager * mManager;

};


#endif