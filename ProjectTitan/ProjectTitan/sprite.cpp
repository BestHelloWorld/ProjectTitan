#include "sprite.h"

#include "utils.h"
#include "program.h"
#include "camera.h"
#include "textbitmap.h"


void ImageSprite::Init(INT program, const CHAR * img_path, FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	mProgram = program;
	mTexture = CreateTexture2DFromBMP(img_path);

	Set(x, y, width, height);
}

void ImageSprite::Set(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	mPos.left = x;
	mPos.right = x + width;
	mPos.top = y + height;
	mPos.bottom = y;

	mVertexBuffer.SetSize(6);

	mVertexBuffer.SetPosition(0, x,			y + height, -1.0f);
	mVertexBuffer.SetPosition(1, x,			y,			-1.0f);
	mVertexBuffer.SetPosition(2, x + width, y,			-1.0f);
	mVertexBuffer.SetPosition(3, x,			y + height, -1.0f);
	mVertexBuffer.SetPosition(4, x + width, y,			-1.0f);
	mVertexBuffer.SetPosition(5, x + width, y + height, -1.0f);

	mVertexBuffer.SetTexcoord(0, 0.0f, 1.0f);
	mVertexBuffer.SetTexcoord(1, 0.0f, 0.0f);
	mVertexBuffer.SetTexcoord(2, 1.0f, 0.0f);
	mVertexBuffer.SetTexcoord(3, 0.0f, 1.0f);
	mVertexBuffer.SetTexcoord(4, 1.0f, 0.0f);
	mVertexBuffer.SetTexcoord(5, 1.0f, 1.0f);

	mVertexBuffer.UpdateData();
	mVertexBuffer.Clear();

	InitVAO();
}

void ImageSprite::InitVAO()
{
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);
	mVertexBuffer.Bind();
	INT location = -1;
	location = glGetAttribLocation(mProgram, SHADER_ATTRIB_POSITION);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)((Vertex *)0)->Position);
	}
	location = glGetAttribLocation(mProgram, SHADER_ATTRIB_TEXCOORD);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)((Vertex *)0)->Texcoord);
	}
	location = glGetAttribLocation(mProgram, SHADER_ATTRIB_NORMAL);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)((Vertex *)0)->Normal);
	}
	location = glGetAttribLocation(mProgram, SHADER_ATTRIB_COLOR);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)((Vertex *)0)->Color);
	}

	mVertexBuffer.Unbind();
	glBindVertexArray(0);
}

void ImageSprite::Draw()
{
	glBindVertexArray(mVertexArray);

	INT location = -1;
	if (mProgram >= 0)
	{
		location = glGetUniformLocation(mProgram, SHADER_UNIFORM_MODEL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, esm::mat4(1.0f).v);
		}

		location = glGetUniformLocation(mProgram, SHADER_UNIFORM_COLOR);
		if (location >= 0)
		{
			glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f);
		}

		location = glGetUniformLocation(mProgram, SHADER_SAMPLER2D_MAIN_TEXTURE);
		if (location >= 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glUniform1i(location, 0);
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer.mVertexesCount);

	glBindVertexArray(0);
}

void SpriteManager::Init(const CHAR * vs, const CHAR * fs, FLOAT screen_width, FLOAT screen_height)
{
	mProgram = new Program;
	mCamera = new Camera;
	mCamera->Init();
	mCamera->Switch2D(screen_width, screen_height);
	mProgram->Init(vs, fs, mCamera);
}

void SpriteManager::New(const CHAR * img_path, FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	ImageSprite * is = new ImageSprite;
	is->Init(mProgram->GetProgramId(), img_path, x, y, width, height);
	mSprites.push_back(is);
}

void SpriteManager::Draw()
{
	mProgram->Bind();

	for (UINT i = 0; i < mSprites.size(); ++i)
	{
		mSprites[i]->Draw();
	}

	mProgram->Unbind();
}


void ButtonManager::Init(const CHAR * vs, const CHAR * fs, FLOAT screen_width, FLOAT screen_height, const CHAR * font_path)
{
	SpriteManager::Init(vs, fs, screen_width, screen_height);

	mText = TextBitmap::GetInstance();
}

void ButtonManager::New(const CHAR * img_path, const WCHAR * text, FLOAT x, FLOAT y, FLOAT width, FLOAT height, std::function<void()> callback)
{
	SpriteManager::New(img_path, x, y, width, height);

	mTexts.push_back(text);
	mFuncs.push_back(callback);
}

void ButtonManager::Draw()
{
	SpriteManager::Draw();

	mText->SetColor(1.0f, 1.0f, 1.0f);

	DEPTH_TEST_END;
	{
		BLEND_BEGIN;
		for (UINT i = 0; i < mTexts.size(); ++i)
		{
			Rectf pos = mSprites[i]->mPos;
			FLOAT half_font_size = (BUTTON_FONT_SZIE) / 2;
			FLOAT x = pos.left + half_font_size;
			FLOAT y = (pos.top + pos.bottom) / 2 - half_font_size;
			mText->Draw(mTexts[i], x, y);
		}
		BLEND_END;
	}
	DEPTH_TEST_BEGIN;
}

void ButtonManager::OnTouchDown(FLOAT x, FLOAT y)
{
}

void ButtonManager::OnTouchUp(FLOAT x, FLOAT y)
{
}

void Button::Init(INT program, const CHAR * img_path, FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	ImageSprite::Init(program, img_path, x, y, width, height);
}

void Button::InitVAO()
{
	ImageSprite::InitVAO();
}

void Button::Set(FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	ImageSprite::Set(x, y, width, height);
}

void Button::Draw()
{
	ImageSprite::Draw();
}
