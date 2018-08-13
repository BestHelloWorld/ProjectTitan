#include "fullscreenquad.h"

#include "program.h"
#include "camera.h"
#include "model.h"

void FullScreenQuad::Init(ScreenLocation location)
{
    mVertexBuffer = new VertexBuffer;
	mProgram = new Program;

    mVertexBuffer->SetSize(4);

	switch (location)
	{
	case LEFT_TOP_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, -1.0f, 1.0f, 0.0f);
		mVertexBuffer->SetPosition(1, -1.0f, 0.0f, 0.0f);
		mVertexBuffer->SetPosition(2, 0.0f, 0.0f, 0.0f);
		mVertexBuffer->SetPosition(3, 0.0f, 1.0f, 0.0f);

		break;
	case LEFT_BOTTOM_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, -1.0f, 0.0f, 0.0f);
		mVertexBuffer->SetPosition(1, -1.0f, -1.0f, 0.0f);
		mVertexBuffer->SetPosition(2, 0.0f, -1.0f, 0.0f);
		mVertexBuffer->SetPosition(3, 0.0f, 0.0f, 0.0f);

		break;
	case RIGHT_TOP_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, 0.0f, 1.0f, 0.0f);
		mVertexBuffer->SetPosition(1, 0.0f, 0.0f, 0.0f);
		mVertexBuffer->SetPosition(2, 1.0f, 0.0f, 0.0f);
		mVertexBuffer->SetPosition(3, 1.0f, 1.0f, 0.0f);

		break;
	case RIGHT_BOTTOM_SCREEN:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, 0.0f, 0.0f, 0.0f);
		mVertexBuffer->SetPosition(1, 0.0f, -1.0f, 0.0f);
		mVertexBuffer->SetPosition(2, 1.0f, -1.0f, 0.0f);
		mVertexBuffer->SetPosition(3, 1.0f, 0.0f, 0.0f);

		break;
	case FULL_SCREEN:
	default:

		mVertexBuffer->SetTexcoord(0, 0.0f, 1.0f);
		mVertexBuffer->SetTexcoord(1, 0.0f, 0.0f);
		mVertexBuffer->SetTexcoord(2, 1.0f, 0.0f);
		mVertexBuffer->SetTexcoord(3, 1.0f, 1.0f);

		mVertexBuffer->SetPosition(0, -1.0f, 1.0f, 0.0f);
		mVertexBuffer->SetPosition(1, -1.0f, -1.0f, 0.0f);
		mVertexBuffer->SetPosition(2, 1.0f, -1.0f, 0.0f);
		mVertexBuffer->SetPosition(3, 1.0f, 1.0f, 0.0f);

		break;
	}

	mVertexBuffer->UpdateData();
	mVertexBuffer->Clear();

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
