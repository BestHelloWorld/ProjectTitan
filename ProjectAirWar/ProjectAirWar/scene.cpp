#include "scene.h"


SceneManager::SceneManager()
{
	mCurrent = NULL;
}


SceneManager::~SceneManager()
{
	auto iter = mScenes.begin();
	while ( iter != mScenes.end())
	{
		delete iter->second;
		iter++;
	}
	mScenes.clear();
}

SceneManager * SceneManager::mManager = new SceneManager();

SceneManager* SceneManager::GetInstance()
{
	return mManager;
}

void SceneManager::InitScenes()
{
	auto iter = mScenes.begin();
	while (iter != mScenes.end())
	{
		iter->second->Init();
		iter++;
	}

	if (mScenes.size() > 0)
	{
		mCurrent = mScenes.begin()->second;
	}
}

void SceneManager::SetViewport(FLOAT width, FLOAT height)
{
	auto iter = mScenes.begin();
	while (iter != mScenes.end())
	{
		iter->second->SetViewport(width, height);
		iter++;
	}
}

void SceneManager::AddScene(const CHAR * sceneName, Scene*scene)
{
	if ((!sceneName) || (!scene))
		return;

	mScenes.insert(ScenePair(sceneName, scene));
}

BOOL SceneManager::Erase(const CHAR * sceneName)
{
	auto iter = mScenes.find(sceneName);
	if (iter == mScenes.end())
	{
		return FALSE;
	}
	delete iter->second;
	mScenes.erase(iter);
	return TRUE;
}

void SceneManager::Next(const CHAR * sceneName)
{
	auto iter = mScenes.find(sceneName);
	if (iter == mScenes.end())
	{
		mCurrent = NULL;
		return;
	}
	mCurrent = iter->second;
}

void SceneManager::Draw(FLOAT s)
{
	if(mCurrent)
		mCurrent->Draw(s);
}

void SceneManager::OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y)
{
	if (mCurrent)
		mCurrent->OnTouch(event, tindex, x, y);
}

void SceneManager::OnKey(UINT event, UCHAR chr)
{
	if (mCurrent)
		mCurrent->OnKey(event, chr);
}