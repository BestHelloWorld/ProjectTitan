#include "scene.h"

#include "framebuffer.h"
#include "program.h"
#include "fullscreenquad.h"
#include "camera.h"


Scene::Scene() :
	mCurrentSM(NULL),
	mDOFTexture(0)
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

void Scene::InitDOF(UINT texture)
{
	mIsOpenDOF = TRUE;
	mDOFTexture = texture;
}

SceneManager * Scene::GetSceneManager()
{
	return mCurrentSM;
}

void Scene::_setSceneManager(SceneManager * sm)
{
	mCurrentSM = sm;
}



DOF * SceneManager::mDOF = new DOF;
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

	mBufFbo = new FrameBuffer;
	mBufFbo->Init();

	mMainFullQuad = new FullScreenQuad;
	mMainFullQuad->Init();

	mSubFullQuad = new FullScreenQuad;
	mSubFullQuad->Init();

	mDOF->mDOF_fullScreen = new FullScreenQuad;
	mDOF->mDOF_fullScreen->Init();
	mDOF->mFbo = new  FrameBuffer;
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
	//mSubFbo->AttachDepthBuffer(FBO_DEPTH, (INT)width, (INT)height);
	mSubFbo->AttachFinish();

	mBufFbo->AttachColorBuffer(FBO_COLOR, GL_COLOR_ATTACHMENT0, (INT)width, (INT)height);
	mBufFbo->AttachDepthBuffer(FBO_DEPTH, (INT)width, (INT)height);
	mBufFbo->AttachFinish();

	mMainFullQuad->SetTexture(mMainFbo->GetBuffer(FBO_COLOR));

	mMainFullQuad->InitBlur(width, height, FULL_SCREEN);

	mViewportWidth = width;
	mViewportHeight = height;
}

void SceneManager::AddScene(const CHAR * sceneName, Scene*scene)
{
	if ((!sceneName) || (!scene))
		return;

	mScenes.insert(std::map<const CHAR *, Scene*>::value_type(sceneName, scene));
	scene->_setSceneManager(this);
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

void SceneManager::SetScene(const CHAR * sceneName)
{
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
	if (size <= 0.0f)
	{
		LOG_E("Error : cannot capture scene.");
		return NULL;
	}

	UCHAR * buf = new UCHAR[size];
	//mMainFbo->Bind();
	glReadnPixels(0, 0, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB, GL_UNSIGNED_BYTE, size, (void*)buf);
	//mMainFbo->Unbind();

	LOG_D("capture width : %d, height : %d  size : %d", (INT)mViewportWidth, (INT)mViewportHeight, size);

	return buf;
}

FLOAT total_distance = 0.f;
void SceneManager::_initTransition()
{
	mTransitionTime = 0.0f;
	total_distance = 0.f;

	UCHAR * buf = CaptureScene();
	mSubFullQuad->SetTexture(buf, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB);
	//mSubFullQuad->SetTexture(NULL, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB);
	mSubFullQuad->Reset(FULL_SCREEN);

	mMainFullQuad->Reset(FULL_SCREEN);
	mMainFullQuad->Move(2.0f, 0.0f, 0.f, FULL_SCREEN);
	delete buf;
}

void SceneManager::_update(FLOAT elapse)
{
	FLOAT move_distance = elapse / 1.0f * TRANSITION_TIME;

	FLOAT C = cos(total_distance) * 2.1f;
	//FLOAT C = pow(1.f - (total_distance / 2.f), 4.0f);
	if (mMainFullQuad->GetX() > .001f)
	{
		total_distance += move_distance;
		mMainFullQuad->Move(-move_distance*C, 0.f, 0.f, FULL_SCREEN);
		mSubFullQuad->Move(-move_distance*C, 0.f, 0.f, FULL_SCREEN);

		LOG_D("mSubFullQuad :: %f, mMainFullQuad :: %f", mSubFullQuad->GetX(), mMainFullQuad->GetX());

		mSubFullQuad->Draw(FALSE);
	}
	else if (mMainFullQuad->GetX())
	{
		total_distance = 0.f;
		mMainFullQuad->Reset(FULL_SCREEN);
	}
}

void SceneManager::Draw(FLOAT s)
{
	CLEAR_COLOR(0.1f, 0.2f, 0.4f);

	mMainFbo->Bind();
	if (mCurrent)
		mCurrent->Draw(s);
	mMainFbo->Unbind();

	_update(s);

	if (mCurrent->mDOFTexture > 0 && mCurrent->mIsOpenDOF)
	{
		//  TEST
		BLEND_BEGIN;
		mMainFullQuad->SetAlphaMap(mMainFbo->GetBuffer(FBO_DEPTH));
		mMainFullQuad->Draw(TRUE);

		mMainFullQuad->SetAlphaMap(0);

		mMainFullQuad->Move(0.f, 0.f, -1.f, FULL_SCREEN);
		mMainFullQuad->Draw(FALSE);
		mMainFullQuad->LoadIdentity();

		BLEND_END;
	}
	else
	{
		mMainFullQuad->Draw(FALSE);
	}
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
