#include "animation.h"

#include "model.h"
#include "camera.h"


void Animation::Init(esm::vec3 startPos, esm::vec3 endPos, FLOAT totalSecond)
{
	mStartPos = startPos, mEndPos = endPos;
	mCurrentPos = mStartPos;
	mAnimDir = mEndPos - mStartPos;
	mTotalTime = totalSecond;
	mCurrentTime = 0.0f;
}


void Animation::Init(esm::vec4 startPos, esm::vec4 endPos, FLOAT totalSecond)
{
	mStartPos = startPos, mEndPos = endPos;
	mCurrentPos = mStartPos;
	mAnimDir = mEndPos - mStartPos;
	mTotalTime = totalSecond;
	mCurrentTime = 0.0f;
}


void Animation::Update(FLOAT second)
{
	RenderableObject::Update(second);

	if (mCurrentTime >= mTotalTime || mStartPos == mEndPos)
	{
		mCurrentTime = mTotalTime;
		return;
	}

	mCurrentTime += second;
	if(mCurrentAnimStatus == NORMAL)
		mCurrentPos = mCurrentPos + mAnimDir * second / mTotalTime;
	else
		mCurrentPos = mCurrentPos - mAnimDir * second / mTotalTime;
	_update();
	std::cout << mCurrentPos.dump() << std::endl;
}


void Animation::_update()
{
	if (mCtlModel)
	{
		mCtlModel->SetPosition(mCurrentPos.x, mCurrentPos.y, mCurrentPos.z);
	}

	if (mCtlCamera)
	{
		mCtlCamera->mEye.x = mCurrentPos.x;
		mCtlCamera->mEye.y = mCurrentPos.y;
		mCtlCamera->mEye.z = mCurrentPos.z;
	}
}


void Animation::Set(Model * model)
{
	mCtlModel = model;
}


void Animation::Set(Camera * camera)
{
	mCtlCamera = camera;
}


void Animation::InverseStart()
{
	mCurrentAnimStatus = INVERSE;
	mCurrentPos = mEndPos;
	mCurrentTime = 0.0f;
}


void Animation::Restart()
{
	mCurrentAnimStatus = NORMAL;
	mCurrentPos = mStartPos;
	mCurrentTime = 0.0f;
}