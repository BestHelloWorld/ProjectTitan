
#ifndef __ANIMATION_H
#define __ANIMATION_H

#include "define.h"
#include "list.hpp"

class Model;
class Camera;

class Animation : public RenderableObject
{
public:
	void Init(esm::vec3 startPos, esm::vec3 endPos, FLOAT totalSecond);
	void Init(esm::vec4 startPos, esm::vec4 endPos, FLOAT totalSecond);
	void Update(FLOAT second);
	void Set(Model * model);
	void Set(Camera * camera);
	void InverseStart();
	void Restart();

	esm::vec4 mStartPos, mEndPos, mAnimDir;
	esm::vec4 mCurrentPos;
	FLOAT mTotalTime, mCurrentTime;

	Model * mCtlModel;
	Camera * mCtlCamera;

	enum ANIMATION_STATUS
	{
		NORMAL, INVERSE
	};
	ANIMATION_STATUS mCurrentAnimStatus;

private:
	void _update();
};

#define _update() _update()

#endif 