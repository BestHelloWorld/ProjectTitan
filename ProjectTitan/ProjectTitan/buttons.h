//
//#ifndef __BUTTONS_H
//#define __BUTTONS_H
//
//#include "define.h"
//
//	typedef struct Rectf
//	{
//		FLOAT left, top, right, bottom;
//	};
//
//typedef struct Button
//{
//	::Rectf* position;
//	UINT vbo;
//	UINT texture;
//	std::function<void()> callback;
//};
//
//class Buttons
//{
//public:
//	void Init(INT program, FLOAT width, FLOAT height);
//	void Draw();
//	void AddButton(const CHAR*texturePath, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, std::function<void()> callback = []()->void{});
//	void AddButton(const UINT texture, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, std::function<void()> callback = []()->void {});
//	void OnTouch(FLOAT x, FLOAT y);
//
//	esm::mat4 mOrthoMatrix;
//	INT mPosLoc, mTexLoc, mUTexLoc, mOrtMatLoc;
//	INT mProgram;
//	std::vector<Button*>* mButtons;
//};
//
//
//#endif
