#ifndef __FULLSCREENQUAD_H
#define __FULLSCREENQUAD_H

#include "define.h"

class Program;
class VertexBuffer;
class FrameBuffer;

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
	void Init(const CHAR * vs, const CHAR * fs, ScreenLocation location = FULL_SCREEN);

	void InitBlur(INT width, INT height, ScreenLocation location = FULL_SCREEN);

	void Draw(BOOL is_drawBlur);
	void SetTexture(UINT texture);
	void SetTexture(UCHAR * texture, INT width, INT height, GLenum format = GL_RGB);
	void SetAlphaMap(UINT texture);

	void Move(FLOAT x, FLOAT y, ScreenLocation location);
	void Set(ScreenLocation location);
	void Reset(ScreenLocation location);

	void SetHorizontalBlur();
	void SetVerticalBlur();

	static UINT GetFullScreenVertexBuffer();

	FLOAT GetX();
	FLOAT GetY();


public:
	Program * mProgram;
private:
	VertexBuffer * mVertexBuffer;

	INT mPosLoc, mTexcLoc;
	INT mUTextLoc;

	UINT mTexture = 0;
	UINT mAlphaMap = 0;

	typedef struct tagPOSITION
	{
		FLOAT x;
		FLOAT y;
	}POSITION;
	POSITION mPos;

	typedef struct tagBLUR
	{
		FrameBuffer * vblur;
		FrameBuffer * hblur;
		bool isBlur = FALSE;
	}BLUR;
	BLUR mBlur;
};

#endif