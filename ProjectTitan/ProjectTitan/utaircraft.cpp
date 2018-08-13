#include "utaircraft.h"

#include "camera.h"


void Aircraft::Init(const CHAR * vs, const CHAR * fs, const CHAR * obj, Camera * cam)
{
	mCamera = NULL;
	Unit::Init(vs, fs, obj, cam);
	mPosition = new cm::vec3(0.0f);
}

void Aircraft::Update(FLOAT second)
{

}

void Aircraft::Roll(FLOAT angle)
{
	mRollAngle += angle;
	if(abs(mRollAngle) > AIRCRAFT_MAX_ROLL_ANGLE)
	{
		mRollAngle = mRollAngle > AIRCRAFT_MAX_ROLL_ANGLE ? AIRCRAFT_MAX_ROLL_ANGLE : mRollAngle;
		mRollAngle = mRollAngle < -AIRCRAFT_MAX_ROLL_ANGLE ? -AIRCRAFT_MAX_ROLL_ANGLE : mRollAngle;
	}
	if(angle == 0)
	{
		if(mRollAngle > 0)
		{
			mRollAngle = mRollAngle - 1.0f;
		}
		else if(mRollAngle < 0)
		{
			mRollAngle = mRollAngle + 1.0f;
		}

		if (abs(mRollAngle) < 1.0f)
			mRollAngle = 0.0f;
	}
	Unit::Rotate(mRollAngle, 0.0f, 0.0f, 1.0f);
}

void Aircraft::BindCamera(Camera * cam)
{
	if (cam == NULL)
		return;

	mCamera = cam;
}

void Aircraft::Move(FLOAT x, FLOAT y, FLOAT z)
{
	z = -1.0f * z;
	x = -1.0f * x;

	z = abs(x) > 0.0f ? abs(x) : z;

	*mPosition = *mPosition + cm::vec3(x, y, z);
	if(mCamera)
	{
		mCamera->mEye = mCamera->mEye + cm::vec3(x, y, z);
		Unit::LoadIdentity();
		Unit::Move(*mPosition);
		Roll(x);
	}
	else
	{
		Unit::Move(x, y, z);
	}
}

void Aircraft::Draw()
{
	Unit::Draw();
}
