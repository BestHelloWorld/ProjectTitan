#ifndef __FULLSCREENQUAD_H
#define __FULLSCREENQUAD_H

#include "define.h"

class Program;
class VertexBuffer;

typedef enum ScreenLocation
{
	FULL_SCREEN,
	LEFT_TOP_SCREEN,
	LEFT_BOTTOM_SCREEN,
	RIGHT_TOP_SCREEN,
	RIGHT_BOTTOM_SCREEN
};

class FullScreenQuad
{
public:
	void Init(ScreenLocation location = FULL_SCREEN);
	void Draw();
	void SetTexture(UINT texture);

private:
	VertexBuffer * mVertexBuffer;
	Program * mProgram;

	INT mPosLoc, mTexcLoc;
	INT mUTextLoc;

	UINT mTexture = -1;
};

#endif