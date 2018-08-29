#ifndef __ANIMATION_H
#define __ANIMATION_H

#include "define.h"
#include "list.hpp"

class Unit;
class Model;
class Camera;

class Animation : public RenderableObject
{
public:
	Animation();
	~Animation();

	void Init(cm::vec3 startPos, cm::vec3 endPos, FLOAT totalSecond = 1.0f);
	void Init(cm::vec4 startPos, cm::vec4 endPos, FLOAT totalSecond = 1.0f);
	void Init(FLOAT startAngle, FLOAT endAngle, FLOAT x, FLOAT y, FLOAT z, FLOAT totalSecond = 1.0f);
	void Update(FLOAT second);
	void Set(Model* model);
	void Set(Camera* camera);
	void Set(Unit* unit);
	void SetEndCallback(void(*func)());
	void Push(List* list);
	void InverseStart();
	void Restart();

	cm::vec4 mStartPos, mEndPos, mAnimDir;
	cm::vec4 mCurrentPos, mMovePos;;
	FLOAT mTotalTime, mCurrentTime;

	Model* mCtlModel;
	Camera* mCtlCamera;
	Unit* mCtlUnit;

	void(*mEndCallback)();

	typedef enum tagANIMATION_STATUS
	{
		NORMAL,
		INVERSE
	}ANIMATION_STATUS;

	ANIMATION_STATUS mCurrentAnimStatus;

	typedef enum tagTRANSFORM
	{
		TRANSLATE,
		ROTATE,
		SCALE
	}TRANSFORM;

	TRANSFORM mCurrentTransform;

private:
	void _init();
	void _update();
};

#define _update() _update()

#endif
