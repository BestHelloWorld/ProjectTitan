#include "animation.h"

#include "model.h"
#include "camera.h"
#include "unit.h"

Animation::Animation()
{
	mCtlModel = NULL;
	mCtlCamera = NULL;
	mCtlUnit = NULL;
}

Animation::~Animation()
{
}

void Animation::_init()
{
	mCurrentAnimStatus = NORMAL;
	mCurrentTime = 0.0f;
	mCurrentPos = cm::vec3(0.0f);
	mMovePos = cm::vec3(0.0f);
	mStartPos = cm::vec3(0.0f);
	mEndPos = cm::vec3(0.0f);
}

void Animation::Init(cm::vec3 startPos, cm::vec3 endPos, FLOAT totalSecond)
{
	_init();
	mStartPos = startPos, mEndPos = endPos;
	mCurrentPos = mStartPos;
	mAnimDir = mEndPos - mStartPos;
	mTotalTime = totalSecond;
	mCurrentTransform = TRANSLATE;
}

void Animation::Init(cm::vec4 startPos, cm::vec4 endPos, FLOAT totalSecond)
{
	_init();
	mStartPos = startPos, mEndPos = endPos;
	mCurrentPos = mStartPos;
	mAnimDir = mEndPos - mStartPos;
	mTotalTime = totalSecond;
	mCurrentTransform = TRANSLATE;
}

void Animation::Init(FLOAT startAngle, FLOAT endAngle, FLOAT x, FLOAT y, FLOAT z, FLOAT totalSecond)
{
	_init();
	mStartPos.x = startAngle;
	mEndPos.x = endAngle;
	mTotalTime = totalSecond;
	mCurrentPos.x = mStartPos.x;
	mAnimDir = cm::vec3(x, y, z);
	mCurrentTransform = ROTATE;
}

void Animation::Update(FLOAT second)
{
	RenderableObject::Update(second);

	if (mCurrentTime >= mTotalTime - second || mStartPos == mEndPos)
	{
		mCurrentTime = mTotalTime;
		return;
	}

	mCurrentTime += second;

	switch(mCurrentTransform)
	{
	case TRANSLATE:
		if (mCurrentAnimStatus == NORMAL)
		{
			mCurrentPos = mCurrentPos + mAnimDir * second / mTotalTime;
			mMovePos = mAnimDir * second / mTotalTime;
		}
		else
		{
			mCurrentPos = mCurrentPos - mAnimDir * second / mTotalTime; 
			mMovePos = mAnimDir * second / mTotalTime * -1.0f;
		}
		break;
	case ROTATE:
		if (mCurrentAnimStatus == NORMAL)
		{
			mCurrentPos.x = mCurrentPos.x + (mEndPos.x - mStartPos.x) * second / mTotalTime;
			mMovePos.x = (mEndPos.x - mStartPos.x) * second / mTotalTime;
		}
		else
		{
			mCurrentPos.x = mCurrentPos.x + (mStartPos.x - mEndPos.x) * second / mTotalTime;
			mMovePos.x = (mStartPos.x - mEndPos.x) * second / mTotalTime;
		}
		break;
	case SCALE:
		break;
	}
	_update();
	//std::cout << mCurrentPos.dump() << std::endl;
	//std::cout << mMovePos.dump() << std::endl;
}


void Animation::_update()
{
	switch (mCurrentTransform)
	{
	case TRANSLATE:
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

		if (mCtlUnit)
		{
			mCtlUnit->Move(mMovePos.x, mMovePos.y, mMovePos.z);
		}
		break;
	case ROTATE:
		if (mCtlUnit)
		{
			mCtlUnit->Rotate(mMovePos.x, mAnimDir.x, mAnimDir.y, mAnimDir.z);
		}
		break;
	case SCALE:
		break;
	}

}


void Animation::Set(Model* model)
{
	mCtlModel = model;
}


void Animation::Set(Camera* camera)
{
	mCtlCamera = camera;
}

void Animation::Set(Unit* unit)
{
	mCtlUnit = unit;
}

void Animation::Push(List* list)
{
	List::Push(list);
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
