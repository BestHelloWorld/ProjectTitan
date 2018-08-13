#ifndef __IMAGE_SPRITE_H
#define __IMAGE_SPRITE_H

#include "define.h"

#include "list.hpp"
#include "model.h"

#define BUTTON_FONT_SZIE 32


typedef struct tagRectf
{
	FLOAT left, top, right, bottom;
}Rectf;

class ImageSprite
{
public:
	virtual void Init(INT program, const CHAR * img_path, FLOAT x, FLOAT y, FLOAT width, FLOAT height);
	virtual void InitVAO();
	virtual void Set(FLOAT x, FLOAT y, FLOAT width, FLOAT height);
	virtual void Draw();

	VertexBuffer mVertexBuffer;
	UINT mVertexArray;
	INT mProgram;
	UINT mTexture;
	Rectf mPos;
};

class Camera;
class Program;

class SpriteManager
{
public:
	virtual void Init(const CHAR * vs, const CHAR * fs, FLOAT screen_width, FLOAT screen_height);
	virtual void New(const CHAR * img_path, FLOAT x, FLOAT y, FLOAT width, FLOAT height);
	virtual void Draw();

	std::vector<ImageSprite*> mSprites;
	Camera * mCamera;
	Program * mProgram;
};

class TextBitmap;

class Button : public ImageSprite
{
public:
	void Init(INT program, const CHAR * img_path, FLOAT x, FLOAT y, FLOAT width, FLOAT height);
	void InitVAO();
	void Set(FLOAT x, FLOAT y, FLOAT width, FLOAT height);
	void Draw();

	BOOL mIsTouchDown;
};

class ButtonManager : public SpriteManager
{
public:
	void Init(const CHAR * vs, const CHAR * fs, FLOAT screen_width, FLOAT screen_height, const CHAR * font_path);
	void New(const CHAR * img_path, const WCHAR * text, FLOAT x, FLOAT y, FLOAT width, FLOAT height, std::function<void()> callback = []()->void {});
	void Draw();

	void OnTouchDown(FLOAT x, FLOAT y);
	void OnTouchUp(FLOAT x, FLOAT y);

	TextBitmap * mText;
	std::vector<const WCHAR*> mTexts;
	std::vector<std::function<void()>> mFuncs;
};

#endif
