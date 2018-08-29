#ifndef __FULLSCREENQUAD_H
#define __FULLSCREENQUAD_H

#include "define.h"

class Program;
class VertexBuffer;

typedef enum tagScreenLocation
{
	FULL_SCREEN,
	LEFT_TOP_SCREEN,
	LEFT_BOTTOM_SCREEN,
	RIGHT_TOP_SCREEN,
	RIGHT_BOTTOM_SCREEN
}ScreenLocation;

class FullScreenQuad
{
public:
	void Init(ScreenLocation location = FULL_SCREEN);
	void Draw();
	void SetTexture(UINT texture);
	void SetTexture(UCHAR * texture, INT width, INT height, GLenum format = GL_RGB);

	void Move(FLOAT x, FLOAT y, ScreenLocation location);
	void Set(ScreenLocation location);
	void Reset(ScreenLocation location);

	FLOAT GetX();
	FLOAT GetY();
private:
	VertexBuffer * mVertexBuffer;
	Program * mProgram;

	INT mPosLoc, mTexcLoc;
	INT mUTextLoc;

	UINT mTexture = -1;

	typedef struct tagPOSITION
	{
		FLOAT x;
		FLOAT y;
	}POSITION;
	POSITION mPos;
};

#endif