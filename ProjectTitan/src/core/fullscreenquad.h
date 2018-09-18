#ifndef __FULLSCREENQUAD_H
#define __FULLSCREENQUAD_H

#include "define.h"

class Program;
class VertexBuffer;
class FrameBuffer;

typedef enum tagSCREEN_LOCATION
{
	FULL_SCREEN,
	LEFT_TOP_SCREEN,
	LEFT_BOTTOM_SCREEN,
	RIGHT_TOP_SCREEN,
	RIGHT_BOTTOM_SCREEN
}SCREEN_LOCATION;

typedef enum tagQUAD_POSITION
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
}QUAD_POSITION;

class FullScreenQuad
{
public:
	void Init(SCREEN_LOCATION location = FULL_SCREEN);
	void Init(const CHAR * vs, const CHAR * fs, SCREEN_LOCATION location = FULL_SCREEN);

	// BLUR
	void InitBlur(INT width, INT height, SCREEN_LOCATION location = FULL_SCREEN);

	void Draw(BOOL is_drawBlur);
	void SetTexture(UINT texture);
	void SetTexture(UCHAR * texture, INT width, INT height, GLenum format = GL_RGB);
	void SetAlphaMap(UINT texture);
	void SetDepthOfField(UINT alpha_map, UINT background_texture);

	Program * GetProgram();

	void SetSide(QUAD_POSITION position, FLOAT top_or_left, FLOAT bottom_or_right);

	void Move(FLOAT x, FLOAT y, FLOAT z, SCREEN_LOCATION location);
	void Set(SCREEN_LOCATION location);
	void SetLocation(SCREEN_LOCATION location);
	void Reset(SCREEN_LOCATION location);
	void LoadIdentity();

	void SetHorizontalBlur();
	void SetVerticalBlur();

	static UINT GetFullScreenVertexBuffer();

	FLOAT & GetX();
	FLOAT & GetY();
	FLOAT & GetZ();


public:
	Program * mProgram;
	SCREEN_LOCATION mScreenLocation;
private:
	VertexBuffer * mVertexBuffer;

	INT mPosLoc, mTexcLoc;
	INT mUTextLoc;

	UINT mTexture = 0;
	UINT mAlphaMap = 0;
	UINT mBackgroundTexture = 0;

	typedef struct tagPOSITION
	{
		FLOAT x;
		FLOAT y;
		FLOAT z;
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