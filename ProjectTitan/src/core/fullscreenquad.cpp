#include "fullscreenquad.h"

#include "program.h"
#include "camera.h"
#include "model.h"
#include "utils.h"
#include "framebuffer.h"

void FullScreenQuad::Init(SCREEN_LOCATION location)
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

	mScreenLocation = location;
	//mUTextLoc = glGetUniformLocation(mProgram->GetProgramId(), "U_Texture");
}

void FullScreenQuad::Init(const CHAR * vs, const CHAR * fs, SCREEN_LOCATION location)
{
	mVertexBuffer = new VertexBuffer;
	mProgram = new Program;

	mVertexBuffer->SetSize(4);
	Reset(location);

	Camera*cam = new Camera;
	cam->Init();
	mProgram->Init(vs, fs, cam);

	mPosLoc = glGetAttribLocation(mProgram->GetProgramId(), "position");
	mTexcLoc = glGetAttribLocation(mProgram->GetProgramId(), "texcoord");

	mScreenLocation = location;
}

void FullScreenQuad::InitBlur(INT width, INT height, SCREEN_LOCATION location)
{
	Init(location);

	mBlur.vblur = new FrameBuffer;
	mBlur.vblur->Init();
	mBlur.vblur->AttachColorBuffer(FBO_COLOR, GL_COLOR_ATTACHMENT0, width, height);
	mBlur.vblur->AttachFinish();

	mBlur.hblur = new FrameBuffer;
	mBlur.hblur->Init();
	mBlur.hblur->AttachColorBuffer(FBO_COLOR, GL_COLOR_ATTACHMENT0, width, height);
	mBlur.hblur->AttachFinish();
}

void FullScreenQuad::Draw(BOOL is_drawBlur)
{
	glBindTexture(GL_TEXTURE_2D, 0);

	if (!is_drawBlur)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mProgram->Bind();
		mVertexBuffer->Bind();

		glEnableVertexAttribArray(mPosLoc);
		glEnableVertexAttribArray(mTexcLoc);
		glVertexAttribPointer(mPosLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, Position));
		glVertexAttribPointer(mTexcLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, Texcoord));

		UINT location = glGetUniformLocation(mProgram->GetProgramId(), SHADER_SAMPLER2D_MAIN_TEXTURE);
		if (location >= 0 && mTexture > 0)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glUniform1i(location, 1);
		}
		location = glGetUniformLocation(mProgram->GetProgramId(), SHADER_SAMPLER2D_ALPHA_TEXTURE);
		if (location >= 0 && mAlphaMap > 0)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, mAlphaMap);
			glUniform1i(location, 2);
		}

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		mVertexBuffer->Unbind();
		mProgram->Unbind();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		UINT mLastTexture = mTexture;

		mBlur.vblur->Bind();
		CLEAR_COLOR(0.f, 0.f, 0.f);
		SetVerticalBlur();
		Draw(FALSE);
		mBlur.vblur->Unbind();


		SetTexture(mBlur.vblur->GetBuffer(FBO_COLOR));
		mBlur.hblur->Bind();
		CLEAR_COLOR(0.f, 0.f, 0.f);
		SetHorizontalBlur();
		Draw(FALSE);
		mBlur.hblur->Unbind();

		CLEAR_COLOR(0.f, 0.f, 0.f);
		mProgram->SetUniform4f("U_Option", 0.f, 1.f, mAlphaMap > 0 ? 1.f : 0.f, 0.f);
		SetTexture(mBlur.hblur->GetBuffer(FBO_COLOR));
		Draw(FALSE);

		mTexture = mLastTexture;
		mProgram->SetUniform4f("U_Option", 0.f, 0.f, 0.f, 0.f);
	}
}


void FullScreenQuad::SetTexture(UINT texture)
{
	mTexture = texture;
}

void FullScreenQuad::SetTexture(UCHAR * texture, INT width, INT height, GLenum format)
{
	if (mTexture <= 0)
		mTexture = CreateTexture2D(texture, width, height, format);
	else
	{
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void FullScreenQuad::SetAlphaMap(UINT texture)
{
	mAlphaMap = texture;
}

void FullScreenQuad::SetSide(QUAD_POSITION position, FLOAT top_or_left, FLOAT bottom_or_right)
{
	switch (mScreenLocation)
	{
	case FULL_SCREEN:

		switch (position)
		{
		case TOP:
			mVertexBuffer->SetPosition(0, top_or_left, mPos.y + 1.f, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, bottom_or_right, mPos.y + 1.f, mPos.z - 1.f);
			break;
		case BOTTOM:
			mVertexBuffer->SetPosition(1, top_or_left, mPos.y - 1.f, mPos.z - 1.f);
			mVertexBuffer->SetPosition(2, bottom_or_right, mPos.y - 1.f, mPos.z - 1.f);
			break;
		case LEFT:
			mVertexBuffer->SetPosition(0, mPos.x - 1.f, top_or_left, mPos.z - 1.f);
			mVertexBuffer->SetPosition(1, mPos.x - 1.f, bottom_or_right, mPos.z - 1.f);
			break;
		case RIGHT:
			mVertexBuffer->SetPosition(2, mPos.x + 1.f, bottom_or_right, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, mPos.x + 1.f, top_or_left, mPos.z - 1.f);
			break;
		default:
			return;
		}

		break;
	case LEFT_TOP_SCREEN:
		switch (position)
		{
		case TOP:
			mVertexBuffer->SetPosition(0, top_or_left, mPos.y + 1.f, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, bottom_or_right, mPos.y + 1.f, mPos.z - 1.f);
			break;
		case BOTTOM:
			mVertexBuffer->SetPosition(1, top_or_left, mPos.y, mPos.z - 1.f);
			mVertexBuffer->SetPosition(2, bottom_or_right, mPos.y, mPos.z - 1.f);
			break;
		case LEFT:
			mVertexBuffer->SetPosition(0, mPos.x - 1.f, top_or_left, mPos.z - 1.f);
			mVertexBuffer->SetPosition(1, mPos.x - 1.f, bottom_or_right, mPos.z - 1.f);
			break;
		case RIGHT:
			mVertexBuffer->SetPosition(2, mPos.x, bottom_or_right, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, mPos.x, top_or_left, mPos.z - 1.f);
			break;
		default:
			return;
		}
		break;
	case LEFT_BOTTOM_SCREEN:
		switch (position)
		{
		case TOP:
			mVertexBuffer->SetPosition(0, top_or_left, mPos.y, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, bottom_or_right, mPos.y, mPos.z - 1.f);
			break;
		case BOTTOM:
			mVertexBuffer->SetPosition(1, top_or_left, mPos.y - 1.f, mPos.z - 1.f);
			mVertexBuffer->SetPosition(2, bottom_or_right, mPos.y - 1.f, mPos.z - 1.f);
			break;
		case LEFT:
			mVertexBuffer->SetPosition(0, mPos.x - 1.f, top_or_left, mPos.z - 1.f);
			mVertexBuffer->SetPosition(1, mPos.x - 1.f, bottom_or_right, mPos.z - 1.f);
			break;
		case RIGHT:
			mVertexBuffer->SetPosition(2, mPos.x, bottom_or_right, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, mPos.x, top_or_left, mPos.z - 1.f);
			break;
		default:
			return;
		}
		break;
	case RIGHT_TOP_SCREEN:
		switch (position)
		{
		case TOP:
			mVertexBuffer->SetPosition(0, top_or_left, mPos.y + 1.f, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, bottom_or_right, mPos.y + 1.f, mPos.z - 1.f);
			break;
		case BOTTOM:
			mVertexBuffer->SetPosition(1, top_or_left, mPos.y, mPos.z - 1.f);
			mVertexBuffer->SetPosition(2, bottom_or_right, mPos.y, mPos.z - 1.f);
			break;
		case LEFT:
			mVertexBuffer->SetPosition(0, mPos.x, top_or_left, mPos.z - 1.f);
			mVertexBuffer->SetPosition(1, mPos.x, bottom_or_right, mPos.z - 1.f);
			break;
		case RIGHT:
			mVertexBuffer->SetPosition(2, mPos.x + 1.f, bottom_or_right, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, mPos.x + 1.f, top_or_left, mPos.z - 1.f);
			break;
		default:
			return;
		}
		break;
	case RIGHT_BOTTOM_SCREEN:
		switch (position)
		{
		case TOP:
			mVertexBuffer->SetPosition(0, top_or_left, mPos.y, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, bottom_or_right, mPos.y, mPos.z - 1.f);
			break;
		case BOTTOM:
			mVertexBuffer->SetPosition(1, top_or_left, mPos.y - 1.f, mPos.z - 1.f);
			mVertexBuffer->SetPosition(2, bottom_or_right, mPos.y - 1.f, mPos.z - 1.f);
			break;
		case LEFT:
			mVertexBuffer->SetPosition(0, mPos.x, top_or_left, mPos.z - 1.f);
			mVertexBuffer->SetPosition(1, mPos.x, bottom_or_right, mPos.z - 1.f);
			break;
		case RIGHT:
			mVertexBuffer->SetPosition(2, mPos.x + 1.f, bottom_or_right, mPos.z - 1.f);
			mVertexBuffer->SetPosition(3, mPos.x + 1.f, top_or_left, mPos.z - 1.f);
			break;
		default:
			return;
		}
		break;
	}

	mVertexBuffer->UpdateData();
}

void FullScreenQuad::Move(FLOAT x, FLOAT y, FLOAT z, SCREEN_LOCATION location)
{
	mPos.x += x;
	mPos.y += y;
	mPos.z += z;
	mScreenLocation = location;
	Set(mScreenLocation);
}

void FullScreenQuad::Set(SCREEN_LOCATION location)
{

	//mVertexBuffer->SetSize(4);
	mScreenLocation = location;
	switch (location)
	{
	case LEFT_TOP_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + -1.0f, mPos.y + 1.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(1, mPos.x + -1.0f, mPos.y + 0.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 0.0f, mPos.y + 0.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 0.0f, mPos.y + 1.0f, mPos.z + -1.0f);

		break;
	case LEFT_BOTTOM_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + -1.0f, mPos.y + 0.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(1, mPos.x + -1.0f, mPos.y + -1.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 0.0f, mPos.y + -1.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 0.0f, mPos.y + 0.0f, mPos.z + -1.0f);

		break;
	case RIGHT_TOP_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + 0.0f, mPos.y + 1.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(1, mPos.x + 0.0f, mPos.y + 0.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 1.0f, mPos.y + 0.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 1.0f, mPos.y + 1.0f, mPos.z + -1.0f);

		break;
	case RIGHT_BOTTOM_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + 0.0f, mPos.y + 0.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(1, mPos.x + 0.0f, mPos.y + -1.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 1.0f, mPos.y + -1.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 1.0f, mPos.y + 0.0f, mPos.z + -1.0f);

		break;
	case FULL_SCREEN:
	default:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, mPos.x + -1.0f, mPos.y + 1.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(1, mPos.x + -1.0f, mPos.y - 1.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(2, mPos.x + 1.0f, mPos.y - 1.0f, mPos.z + -1.0f);
		mVertexBuffer->SetPosition(3, mPos.x + 1.0f, mPos.y + 1.0f, mPos.z + -1.0f);

		break;
	}

	mVertexBuffer->UpdateData();
	//mVertexBuffer->Clear();
}

void FullScreenQuad::Reset(SCREEN_LOCATION location)
{
	mPos = { 0.f, 0.f, 0.f };
	mScreenLocation = location;
	Set(mScreenLocation);
}

void FullScreenQuad::LoadIdentity()
{
	mPos = { 0.f, 0.f, 0.f };
	Set(mScreenLocation);
}

void FullScreenQuad::SetHorizontalBlur()
{
	mProgram->SetUniform4f("U_Option", 1.f, 0.f, 0.f, 0.f);
}

void FullScreenQuad::SetVerticalBlur()
{
	mProgram->SetUniform4f("U_Option", 0.f, 1.f, 0.f, 0.f);
}

FLOAT & FullScreenQuad::GetX()
{
	return mPos.x;
}

FLOAT & FullScreenQuad::GetY()
{
	return mPos.y;
}

FLOAT & FullScreenQuad::GetZ()
{
	return mPos.z;
}
