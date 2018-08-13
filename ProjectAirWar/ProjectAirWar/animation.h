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

	void Init(esm::vec3 startPos, esm::vec3 endPos, FLOAT totalSecond = 1.0f);
	void Init(esm::vec4 startPos, esm::vec4 endPos, FLOAT totalSecond = 1.0f);
	void Init(FLOAT startAngle, FLOAT endAngle, FLOAT x, FLOAT y, FLOAT z, FLOAT totalSecond = 1.0f);
	void Update(FLOAT second);
	void Set(Model* model);
	void Set(Camera* camera);
	void Set(Unit* unit);
	void Push(List* list);
	void InverseStart();
	void Restart();

	esm::vec4 mStartPos, mEndPos, mAnimDir;
	esm::vec4 mCurrentPos, mMovePos;;
	FLOAT mTotalTime, mCurrentTime;

	Model* mCtlModel;
	Camera* mCtlCamera;
	Unit* mCtlUnit;

	typedef enum ANIMATION_STATUS
	{
		NORMAL,
		INVERSE
	};

	ANIMATION_STATUS mCurrentAnimStatus;

	typedef enum TRANSFORM
	{
		TRANSLATE,
		ROTATE,
		SCALE
	};

	TRANSFORM mCurrentTransform;

private:
	void _init();
	void _update();
};

#define _update() _update()

#endif
