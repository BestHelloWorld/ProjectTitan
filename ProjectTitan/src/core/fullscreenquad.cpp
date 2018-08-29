#include "fullscreenquad.h"

#include "program.h"
#include "camera.h"
#include "model.h"
#include "utils.h"

void FullScreenQuad::Init(ScreenLocation location)
{
    mVertexBuffer = new VertexBuffer;
	mProgram = new Program;

	mVertexBuffer->SetSize(4);
	Reset(location);

	Camera*cam = new Camera;
	cam->Init();
	mProgram->Init("res/shader/fullscreenquad.vs", "res/shader/fullscreenquad.fs", cam);

	mPosLoc = glGetAttribLocation(mProgram->GetProgramId(), "position");
	mTexcLoc = glGetAttribLocation(mProgram->GetProgramId(), "texcoord");
	mUTextLoc = glGetUniformLocation(mProgram->GetProgramId(), "U_Texture");
}

void FullScreenQuad::Draw()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mProgram->Bind();
    mVertexBuffer->Bind();

	glEnableVertexAttribArray(mPosLoc);
	glEnableVertexAttribArray(mTexcLoc);
	glVertexAttribPointer(mPosLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((Vertex*)0)->Position);
	glVertexAttribPointer(mTexcLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((Vertex*)0)->Texcoord);

	if (mUTextLoc >= 0 && mTexture != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glUniform1i(mUTextLoc, 1);
	}

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	mVertexBuffer->Unbind();
	mProgram->Unbind();
}


void FullScreenQuad::SetTexture(UINT texture)
{
	mTexture = texture;
}

void FullScreenQuad::SetTexture(UCHAR * texture, INT width, INT height, GLenum format)
{
	if (mTexture == -1)
		mTexture = CreateTexture2D(texture, width, height, format);
	else
	{
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void FullScreenQuad::Move(FLOAT x, FLOAT y, ScreenLocation location)
{
	mPos.x += x;
	mPos.y += y;
	Set(location);
}

void FullScreenQuad::Set(ScreenLocation location)
{

	//mVertexBuffer->SetSize(4);
	switch (location)
	{
	case LEFT_TOP_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + -1.0f, mPos.y + 1.0f, 0.0f);
		mVertexBuffer->SetPosition(1, mPos.x + -1.0f, mPos.y + 0.0f, 0.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 0.0f, mPos.y + 0.0f, 0.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 0.0f, mPos.y + 1.0f, 0.0f);

		break;
	case LEFT_BOTTOM_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + -1.0f, mPos.y + 0.0f, 0.0f);
		mVertexBuffer->SetPosition(1, mPos.x + -1.0f, mPos.y + -1.0f, 0.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 0.0f, mPos.y + -1.0f, 0.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 0.0f, mPos.y + 0.0f, 0.0f);

		break;
	case RIGHT_TOP_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + 0.0f, mPos.y + 1.0f, 0.0f);
		mVertexBuffer->SetPosition(1, mPos.x + 0.0f, mPos.y + 0.0f, 0.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 1.0f, mPos.y + 0.0f, 0.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 1.0f, mPos.y + 1.0f, 0.0f);

		break;
	case RIGHT_BOTTOM_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + 0.0f, mPos.y + 0.0f, 0.0f);
		mVertexBuffer->SetPosition(1, mPos.x + 0.0f, mPos.y + -1.0f, 0.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 1.0f, mPos.y + -1.0f, 0.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 1.0f, mPos.y + 0.0f, 0.0f);

		break;
	case FULL_SCREEN:
	default:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + -1.0f, mPos.y + 1.0f, -1.0f);
		mVertexBuffer->SetPosition(1, mPos.x + -1.0f, mPos.y + -1.0f, -1.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 1.0f, mPos.y + -1.0f, -1.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 1.0f, mPos.y + 1.0f, -1.0f);

		break;
	}

	mVertexBuffer->UpdateData();
	//mVertexBuffer->Clear();
}

void FullScreenQuad::Reset(ScreenLocation location)
{
	mPos = { 0.0f, 0.0f };
	Set(location);
}

FLOAT FullScreenQuad::GetX()
{
	return mPos.x;
}

FLOAT FullScreenQuad::GetY()
{
	return mPos.y;
}
