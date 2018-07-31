#ifndef __FULLSCREENQUAD_H
#define __FULLSCREENQUAD_H

#include "define.h"

class Program;
class VertexBuffer;

class FullScreenQuad
{
public:
	void Init();
	void Draw();
	void SetTexture(UINT texture);

	void DrawToLeftTop();
	void DrawToRightTop();
	void DrawToLeftBottom();
	void DrawToRightBottom();
public:
	VertexBuffer * mVertexBuffer;
	Program * mProgram;

	INT mPosLoc, mTexcLoc;
	INT mUTextLoc;

	UINT mTexture = -1;
};

#endif