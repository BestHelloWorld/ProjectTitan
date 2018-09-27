#include "scene.h"

#include "framebuffer.h"
#include "program.h"
#include "fullscreenquad.h"
#include "camera.h"

#define _MAX_MOVE_LEFT		-1.0f
#define _MAX_MOVE_RIGHT		1.0f

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
void Scene::OnResume()
{
	LOG_D("Scene on resume");
}
void Scene::OnPause()
{
	LOG_D("Scene on pause");
}
void Scene::Draw(FLOAT s)
{
}

UINT Scene::GetDepthBuffer()
{
	SceneManager * _sm = GetSceneManager();
	if (!_sm)
		return 0;
	return _sm->GetDepthBuffer();
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
	mSubFbo->AttachDepthBuffer(FBO_DEPTH, (INT)width, (INT)height);
	mSubFbo->AttachFinish();

	mBufFbo->AttachColorBuffer(FBO_COLOR, GL_COLOR_ATTACHMENT0, (INT)width, (INT)height);
	mBufFbo->AttachDepthBuffer(FBO_DEPTH, (INT)width, (INT)height);
	mBufFbo->AttachFinish();

	mMainFullQuad->SetTexture(mMainFbo->GetBuffer(FBO_COLOR));

	mMainFullQuad->InitBlur((INT)width, (INT)height, FULL_SCREEN);

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

	SetScene(sceneName);
}

FLOAT SceneManager::GetViewportWidth()
{
	return mViewportWidth;
}

FLOAT SceneManager::GetViewportHeight()
{
	return mViewportHeight;
}

FrameBuffer * SceneManager::GetFrameBuffer()
{
	return mMainFbo;
}

void SceneManager::SetScene(const CHAR * sceneName)
{
	auto iter = mScenes.find(sceneName);
	if (iter == mScenes.end())
	{
		mCurrent = NULL;
		return;
	}
	if (mCurrent)
		mCurrent->OnPause();
	mCurrent = iter->second;
	mCurrent->OnResume();
}

Scene * SceneManager::GetScene(const CHAR * sceneName)
{
	return mScenes.find(sceneName)->second;
}

UINT SceneManager::GetColorBuffer()
{
	return mMainFbo->GetBuffer(FBO_COLOR);
}

UINT SceneManager::GetDepthBuffer()
{
	return mMainFbo->GetBuffer(FBO_DEPTH);
}

void * SceneManager::CaptureScene(UINT capture_buffer)
{

	switch (capture_buffer)
	{
	case CAPTURE_COLOR:
	{
		INT size = (INT)mViewportWidth * (INT)mViewportHeight * 3;
		if (size <= 0.0f)
		{
			LOG_E("Error : cannot capture scene.");
			return NULL;
		}

		UCHAR * buf = new UCHAR[size];
		glReadnPixels(0, 0, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB, GL_UNSIGNED_BYTE, size, (void*)buf);
		LOG_D("capture color :: width : %d, height : %d  size : %d", (INT)mViewportWidth, (INT)mViewportHeight, size);
		return buf;
	}
	case CAPTURE_DEPTH:
	{
		//INT size = (INT)mViewportWidth * (INT)mViewportHeight;
		//if (size <= 0.0f)
		//{
		//	LOG_E("Error : cannot capture scene.");
		//	return NULL;
		//}
		//FLOAT * buf = new FLOAT[size];
		//glReadnPixels(0, 0, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB, GL_UNSIGNED_BYTE, size, (void*)buf);
		//LOG_D("capture depth :: width : %d, height : %d  size : %d", (INT)mViewportWidth, (INT)mViewportHeight, size);
		//return buf;
	}
	}

	return NULL;
}

FLOAT _total_distance = 0.f;
FLOAT _real_total_distance = 0.f;
FLOAT _check_update = 0.f;

void SceneManager::_initTransition()
{
	mTransitionTime = 0.0f;

	_total_distance = 0.f;
	_real_total_distance = 0.f;
	_check_update = 2.0f;

#define CAPTURE_BUFFER CAPTURE_COLOR
	UCHAR * buf = (UCHAR*)CaptureScene();
	mSubFullQuad->SetTexture(buf, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB);
	//mSubFullQuad->SetTexture(NULL, (INT)mViewportWidth, (INT)mViewportHeight, GL_RGB);
	mSubFullQuad->Reset(FULL_SCREEN);

	mMainFullQuad->Reset(FULL_SCREEN);

#if TRANSITION_STYLE == TRANSFORM_TRANSLATE
	mMainFullQuad->Move(_check_update, 0.0f, 0.f, FULL_SCREEN);
#endif

	delete buf;
}


void SceneManager::_update(FLOAT elapse)
{
	FLOAT move_distance = elapse / 1.f * TRANSITION_TIME;

	FLOAT C = cos(_total_distance) * 2.1f;
	//FLOAT C = pow(1.f - (total_distance / 2.f), 4.0f);
	if (_check_update > .001f)
	{
		FLOAT _md = -move_distance*C;

		_total_distance += move_distance;
		_real_total_distance += _md;

		FLOAT complete_rate = abs(_real_total_distance) * .5f;

		_check_update += _md;

#if TRANSITION_STYLE == TRANSFORM_CUBE_ROTATE
		const FLOAT MIN_SIDE = .4f;
		FLOAT main_side = complete_rate * (1.f - MIN_SIDE) + MIN_SIDE;

		mMainFullQuad->SetSide(TOP, _real_total_distance + 1.f, 1.f);
		mMainFullQuad->SetSide(BOTTOM, _real_total_distance + 1.f, 1.f);
		mMainFullQuad->SetSide(RIGHT, main_side * 1.f, -main_side * 1.f);
		//mMainFullQuad->GetX() += _md;

		FLOAT sub_side = (1.0f - complete_rate) * (1.0f - MIN_SIDE) + MIN_SIDE;
		mSubFullQuad->SetSide(TOP, -1.f, _real_total_distance + 1.f);
		mSubFullQuad->SetSide(BOTTOM, -1.f, _real_total_distance + 1.f);
		mSubFullQuad->SetSide(LEFT, sub_side * 1.f, -sub_side * 1.f);
#elif TRANSITION_STYLE == TRANSFORM_TRANSLATE
		mMainFullQuad->Move(_md, 0.f, 0.f, FULL_SCREEN);
		mSubFullQuad->Move(_md, 0.f, 0.f, FULL_SCREEN);
#endif

		//LOG_D("Complete :: %f", complete_rate);

		mSubFullQuad->Draw(FALSE);
	}
	else if (mMainFullQuad->GetX())
	{
		_total_distance = 0.f;
		_real_total_distance = 0.f;

		mMainFullQuad->Reset(FULL_SCREEN);
	}
}

void SceneManager::Draw(FLOAT s)
{
	CLEAR_COLOR(.1f, .2f, .4f);

	mMainFbo->Bind();
	if (mCurrent)
		mCurrent->Draw(s);
	mMainFbo->Unbind();

	_update(s);

	if (mCurrent->mDOFTexture > 0 && mCurrent->mIsOpenDOF)
	{
		// DEPTH OF FIELD TEST
		mBufFbo->Bind();
		CLEAR_COLOR(0.1f, 0.2f, 0.4f);
		mMainFullQuad->SetTexture(GetColorBuffer());
		mMainFullQuad->Draw(FALSE);
		mBufFbo->Unbind();

		mMainFullQuad->SetDepthOfField(mCurrent->mDOFTexture, mBufFbo->GetBuffer(FBO_COLOR));
		mMainFullQuad->mProgram->SetUniform4f("U_Option", 0.0f, 0.0f, 1.0f, 0.0f);
		mMainFullQuad->Draw(TRUE);
		mMainFullQuad->mProgram->SetUniform4f("U_Option", 0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		//mMainFullQuad->SetTexture(mMainFbo->GetBuffer(FBO_DEPTH));
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
