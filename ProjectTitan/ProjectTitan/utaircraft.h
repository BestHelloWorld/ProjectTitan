#ifndef __AIRCRAFT_H
#define __AIRCRAFT_H


#include "define.h"

#include "unit.h"

#define AIRCRAFT_MAX_ROLL_ANGLE 45.0f

class Aircraft : public Unit
{
public:
	void Init(const CHAR * vs, const CHAR * fs, const CHAR * obj, Camera * cam);
	void Update(FLOAT time);
	void Roll(FLOAT angle);
	void BindCamera(Camera * cam);
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void Draw();

	cm::vec3 * mPosition;
	FLOAT mRollAngle;

	Camera * mCamera;
};

#endif
