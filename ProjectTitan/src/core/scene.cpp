#include "scene.h"

#include "framebuffer.h"
#include "program.h"
#include "fullscreenquad.h"


Scene::Scene()
{
}
Scene::~Scene()
{
}

void Scene::Init()
{

}

void Scene::SetViewport(FLOAT width, FLOAT height)
{

}
void Scene::OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y)
{

}
void Scene::OnKey(UINT event, UCHAR chr)
{

}
void Scene::Draw(FLOAT s)
{

}


SceneManager::SceneManager()
{
	mCurrent = NULL;
}


SceneManager::~SceneManager()
{
	auto iter = mScenes.begin();
	while (iter != mScenes.end())
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

	mFboProg = new Program;
	mFboProg->Init("res/shader/fullscreenquad.vs", "res/shader/fullscreenquad.fs", NULL);

	mMainFbo = new FrameBuffer;
	mMainFbo->Init();

	mSubFbo = new FrameBuffer;
	mSubFbo->Init();

	mMainFullQuad = new FullScreenQuad;
	mMainFullQuad->Init();

	mSubFullQuad = new FullScreenQuad;
	mSubFullQuad->Init();
}

void SceneManager::SetViewport(FLOAT width, FLOAT height)
{
	auto iter = mScenes.begin();
	while (iter != mScenes.end())
	{
		iter->second->SetViewport(width, height);
		iter++;
	}


	mMainFbo->AttachColorBuffer(FBO_COLOR, GL_COLOR_ATTACHMENT0, (INT)width, (INT)height);
	mMainFbo->AttachDepthBuffer(FBO_DEPTH, (INT)width, (INT)height);
	mMainFbo->AttachFinish();

	mSubFbo->AttachColorBuffer(FBO_COLOR, GL_COLOR_ATTACHMENT0, (INT)width, (INT)height);
	mSubFbo->AttachDepthBuffer(FBO_DEPTH, (INT)width, (INT)height);
	mSubFbo->AttachFinish();

	mMainFullQuad->SetTexture(mMainFbo->GetBuffer(FBO_COLOR));

	mViewportWidth = width;
	mViewportHeight = height;
}

void SceneManager::AddScene(const CHAR * sceneName, Scene*scene)
{
	if ((!sceneName) || (!scene))
		return;

	mScenes.insert(std::pair<const CHAR *, Scene*>(sceneName, scene));
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
	LOG_D("Next : %s", sceneName);

	_initTransition();

	auto iter = mScenes.find(sceneName);
	if (iter == mScenes.end())
	{
		mCurrent = NULL;
		return;
	}
	mCurrent = iter->second;
}

Scene * SceneManager::GetScene(const CHAR * sceneName)
{
	return mScenes.find(sceneName)->second;
}

UCHAR * SceneManager::CaptureScene()
{
	INT size = (INT)mViewportWidth * (INT)mViewportHeight * 3;
	if (size < 0.0f || size > 50000000.0f)
		return NULL;

	UCHAR * buf = new UCHAR[size];
	mMainFbo->Bind();
	glReadnPixels(0, 0, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB, GL_UNSIGNED_BYTE, size, (void*)buf);
	mMainFbo->Unbind();

	LOG_D("capture width : %d, height : %d  size : %d", (INT)mViewportWidth, (INT)mViewportHeight, size);

	return buf;
}

void SceneManager::_initTransition()
{
	mTransitionTime = 0.0f;

	UCHAR * buf = CaptureScene();
	mSubFullQuad->SetTexture(buf, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB);
	//mSubFullQuad->SetTexture(NULL, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB);
	mSubFullQuad->Reset(FULL_SCREEN);
	delete buf;

	mMainFullQuad->Reset(FULL_SCREEN);
	mMainFullQuad->Move(2.0f, 0.0f, FULL_SCREEN);
}

FLOAT total_distance = 0.f;
void SceneManager::_update(FLOAT elapse)
{
	FLOAT move_distance = elapse / 1.0f * TRANSITION_TIME;

	FLOAT C = cos(total_distance) * 2.1f;
	//FLOAT C = pow(1.f - (total_distance / 2.f), 4.0f);
	if (mMainFullQuad->GetX() > .001f)
	{
		total_distance += move_distance;
		mMainFullQuad->Move(-move_distance*C, 0.0f, FULL_SCREEN);
		mSubFullQuad->Move(-move_distance*C, 0.0f, FULL_SCREEN);

		mSubFullQuad->Draw();
	}
	else if (mMainFullQuad->GetX())
	{
		total_distance = 0.f;
		mMainFullQuad->Reset(FULL_SCREEN);
	}
}

void SceneManager::Draw(FLOAT s)
{
	mMainFbo->Bind();

	if (mCurrent)
		mCurrent->Draw(s);

	mMainFbo->Unbind();

	CLEAR_COLOR(0.0f, 0.0f, 0.0f);

	_update(s);
	mMainFullQuad->Draw();
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
