#include "buttons.h"

#include "program.h"
#include "model.h"
#include "utils.h"

void Buttons::Init(INT program, FLOAT width, FLOAT height)
{
	mProgram = program;

	if (mProgram > 0)
	{
		mPosLoc = glGetAttribLocation(mProgram, "position");
		mTexLoc = glGetAttribLocation(mProgram, "texcoord");
		mOrtMatLoc = glGetUniformLocation(mProgram, "ProjectMatrix");
		mUTexLoc = glGetUniformLocation(mProgram, "U_Texture");
	}

	FLOAT halfWidth = width / 2;
	FLOAT halfHeight = height / 2;
	mOrthoMatrix = esm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 100.0f).transpose();
}
void Buttons::Draw()
{
	if (mProgram == NEGATIVE_ONE)
		return;

	glUseProgram(mProgram);
	for (int i = 0; i < mButtons->size(); ++i)
	{
		UINT vbo = (*mButtons)[i]->vbo;
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		INT location = glGetAttribLocation(mProgram, SHADER_ATTRIB_POSITION);
		if (location >= 0)
		{
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
		}
		location = glGetAttribLocation(mProgram, SHADER_ATTRIB_TEXCOORD);
		if (location >= 0)
		{
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(FLOAT) * 4));
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		location = glGetAttribLocation(mProgram, SHADER_UNIFORM_MODEL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, esm::value_ptr(esm::mat4(1.0f)));
		}
		location = glGetAttribLocation(mProgram, SHADER_UNIFORM_VIEW_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, esm::value_ptr(esm::mat4(1.0f)));
		}
		location = glGetAttribLocation(mProgram, SHADER_UNIFORM_PROJECTION_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, esm::value_ptr(mOrthoMatrix));
		}
		if (mUTexLoc >= 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, (*mButtons)[i]->texture);
			glUniform1i(mUTexLoc, 0);
		}
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	glUseProgram(0);

}
void Buttons::AddButton(const CHAR*path, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, std::function<void()> callback)
{
	UINT texture;
	texture = CreateTexture2DFromBMP(path);
	const FLOAT vertexes[] =
	{
		left , top, -1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
		left, bottom, -1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,
		right, bottom, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		right, top, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f
	};

	UINT vbo = VertexBuffer::CreateVertexBufferObject(GL_ARRAY_BUFFER, sizeof(float) * 8 * 4, GL_STATIC_DRAW, (void*)vertexes);

	Button* btn = new Button;
	btn->position = new ::RECTF;
	{
		btn->position->left = left;
		btn->position->top = top;
		btn->position->right = right;
		btn->position->bottom = bottom;
	}
	btn->vbo = vbo;
	btn->texture = texture;
	btn->callback = callback;

	mButtons->push_back(btn);
}

void Buttons::AddButton(const UINT texture, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, std::function<void()> callback)
{
	const FLOAT vertexes[] =
	{
		left , top, -1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,
		left, bottom, -1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,
		right, bottom, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		right, top, -1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f
	};

	UINT vbo = VertexBuffer::CreateVertexBufferObject(GL_ARRAY_BUFFER, sizeof(float) * 8 * 4, GL_STATIC_DRAW, (void*)vertexes);

	Button* btn = new Button;
	btn->position = new ::RECTF;
	{
		btn->position->left = left;
		btn->position->top = top;
		btn->position->right = right;
		btn->position->bottom = bottom;
	}
	btn->vbo = vbo;
	btn->texture = texture;
	btn->callback = callback;

	mButtons->push_back(btn);
}

void Buttons::OnTouch(FLOAT x, FLOAT y)
{
	LOG_D("click x:%f, y:%f", x, y);

	for (int i = 0; i < mButtons->size(); ++i)
	{
		::LPRECTF pos = (*mButtons)[i]->position;
		if (x > pos->left && x < pos->right && y < pos->top && y > pos->bottom)
		{
			(*mButtons)[i]->callback();
			return;
		}
	}
}