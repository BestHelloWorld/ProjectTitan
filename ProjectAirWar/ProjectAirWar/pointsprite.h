#ifndef __POINTSPRITE_H
#define __POINTSPRITE_H

#include "define.h"

#define POINT_SPRITE_SIZE 10.0f
#define POINT_SPRITE_TIME 10.0f

class PointSprite
{
public:
	PointSprite();
	~PointSprite();

	void Init(INT program, esm::vec3 pos, esm::vec3 moveDir);
	void SetColor(FLOAT r, FLOAT g, FLOAT b);
	void Move(FLOAT x, FLOAT y, FLOAT z);
	void Move(esm::vec3 pos);
	void Move(FLOAT distance);
	void Draw();

	INT mProgram;
	INT mPointSize;
	FLOAT mTime;
	esm::vec3 * mPosition;
	esm::vec4 * mColor;
	esm::vec3 * mMoveDir;
};

class Camera;
class Program;

class SpriteEmit
{
public:
	void Init(const CHAR * vs, const CHAR * fs, esm::vec3 emitDir, Camera * cam, FLOAT newSpritePerSecond = 10.0f, FLOAT spriteSpeed = 2.0f, BOOL autoEmit = TRUE);
	void Update(FLOAT second);
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void SetPosition(esm::vec3 pos);
	void Emit();
	virtual void Draw();

	esm::vec3 *mEmitDir, *mEmitPos;
	Program * mProgram = NULL;
	FLOAT mNewSpritePerSecond,mSpriteSpeed, mCurrentTime;
	std::vector<PointSprite*> mSprites;
	BOOL mAutoEmit;
};

#endif