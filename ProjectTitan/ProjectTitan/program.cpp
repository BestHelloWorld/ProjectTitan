#include "program.h"
#include "camera.h"
#include "utils.h"
#include "model.h"

UINT Program::CompileShader(ENUM shaderType, const CHAR *shaderCode)
{
	GLuint shader = 0;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, 0);
	glCompileShader(shader);

	GLint compileRes;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileRes);
	if (compileRes == GL_FALSE)
	{
		char *buf = new char[1024];
		int len;
		glGetShaderInfoLog(shader, 1024, &len, buf);
		if (len > 0)
		{
			LOG_E("Shader Compile failed : %s", buf);
		}
		delete buf;
		return -1;
	}
	return shader;
}

UINT Program::CreateProgram(UINT vsShader, UINT fsShader)
{
	if (vsShader == -1 || fsShader == -1)
	{
		return -1;
	}
	UINT program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);

	glLinkProgram(program);

	GLint linkRes;
	glGetProgramiv(program, GL_LINK_STATUS, &linkRes);
	if (linkRes == GL_FALSE)
	{
		char *buf = new char[1024];
		int len;
		glGetProgramInfoLog(program, 1024, &len, buf);
		if (len > 0)
		{
			LOG_E("Program Link failed : %s", buf);
		}
		delete buf;
		return -1;
	}
	return program;
}

void Program::Init(const CHAR *vsPath, const CHAR *fsPath, Camera*camera)
{
	mCamera = camera;

	INT nFileSize = 0;
	const CHAR *vsCode = (CHAR*)LoadFileContent(vsPath, nFileSize);
	const CHAR *fsCode = (CHAR *)LoadFileContent(fsPath, nFileSize);

	UINT vsShader = CompileShader(GL_VERTEX_SHADER, vsCode);
	UINT fsShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);
	if ((!vsShader) || (!fsShader))
	{
		return;
	}

	mProgram = CreateProgram(vsShader, fsShader);
	if (mProgram != 0)
	{
		//mModelMatrixLocation = glGetUniformLocation(mProgram, SHADER_UNIFORM_MODEL_MATRIX);
		mViewMatrixLocation = glGetUniformLocation(mProgram, SHADER_UNIFORM_VIEW_MATRIX);
		mProjectMatrixLocation = glGetUniformLocation(mProgram, SHADER_UNIFORM_PROJECTION_MATRIX);

	}
}

void Program::Bind()
{
	if (!mCamera)
		return;

	glUseProgram(mProgram);
	if (mViewMatrixLocation >= 0)
		glUniformMatrix4fv(mViewMatrixLocation, 1, GL_TRUE, mCamera->GetViewMatrix());
	if (mProjectMatrixLocation >= 0)
		glUniformMatrix4fv(mProjectMatrixLocation, 1, GL_TRUE, mCamera->GetProjectionMatrix());


	//if (mViewMatrixLocation >= 0)
	//	glUniformMatrix4fv(mViewMatrixLocation, 1, GL_FALSE, esm::value_ptr(*camera.GetViewMatrix()));
	//if (mProjectMatrixLocation >= 0)
	//	glUniformMatrix4fv(mProjectMatrixLocation, 1, GL_FALSE, esm::value_ptr(*camera.GetProjectionMatrix()));

	//std::cout << camera.GetViewMatrix()->dump() << std::endl;
	//std::cout << camera.GetProjectionMatrix()->dump() << std::endl;


	INT index = 0;
	for (auto iter = mUniformTextures.begin(); iter != mUniformTextures.end(); iter++)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, iter->second->texture);
		glUniform1i(iter->second->location, index++);
	}
	for (auto iter = mUniformTextureCubes.begin(); iter != mUniformTextureCubes.end(); iter++)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, iter->second->texture);
		glUniform1i(iter->second->location, index++);
	}
	for (auto iter = mUniformVectors.begin(); iter != mUniformVectors.end(); iter++)
	{
		glUniform4fv(iter->second->location, 1, iter->second->v);
	}


}

void Program::Unbind()
{
	glUseProgram(0);
}

void Program::SetTexture(const CHAR *name, const CHAR *path)
{
	auto iter = mUniformTextures.find(name);
	if (iter == mUniformTextures.end())
	{
		INT location = glGetUniformLocation(mProgram, name);
		if (location != -1)
		{
			UniformTexture *t = new UniformTexture;
			t->location = location;
			t->texture = CreateTexture2DFromBMP(path);
			mUniformTextures.insert(std::pair<std::string, UniformTexture *>(name, t));
		}
	}
	else
	{
		glDeleteTextures(1, &(iter->second->texture));
		iter->second->texture = CreateTexture2DFromBMP(path);
	}
}

UINT Program::SetTexture(const CHAR *name, UINT texture)
{
	UINT oldTexture = 0;
	auto iter = mUniformTextures.find(name);
	if (iter == mUniformTextures.end())
	{
		INT location = glGetUniformLocation(mProgram, name);
		if (location != -1)
		{
			UniformTexture *t = new UniformTexture;
			t->location = location;
			t->texture = texture;
			mUniformTextures.insert(std::pair<std::string, UniformTexture *>(name, t));
		}
	}
	else
	{
		oldTexture = iter->second->texture;
		iter->second->texture = texture;
	}
	return oldTexture;
}

UINT Program::SetTextureCube(const CHAR *name, UINT texture)
{
	UINT oldTexture = 0;
	auto iter = mUniformTextureCubes.find(name);
	if (iter == mUniformTextureCubes.end())
	{
		INT location = glGetUniformLocation(mProgram, name);
		if (location != -1)
		{
			UniformTextureCube *t = new UniformTextureCube;
			t->location = location;
			t->texture = texture;
			mUniformTextureCubes.insert(std::pair<std::string, UniformTextureCube *>(name, t));
		}
	}
	else
	{
		oldTexture = iter->second->texture;
		iter->second->texture = texture;
	}
	return oldTexture;
}

void Program::SetVector4f(const CHAR *name, FLOAT x, FLOAT y, FLOAT z, FLOAT w)
{
	auto iter = mUniformVectors.find(name);
	if (iter == mUniformVectors.end())
	{
		INT location = glGetUniformLocation(mProgram, name);
		if (location != -1)
		{
			UniformVector4f *v = new UniformVector4f;
			v->location = location;
			v->v[0] = x;
			v->v[1] = y;
			v->v[2] = z;
			v->v[3] = w;
			mUniformVectors.insert(std::pair<std::string, UniformVector4f *>(name, v));
		}
	}
	else
	{
		iter->second->v[0] = x;
		iter->second->v[1] = y;
		iter->second->v[2] = z;
		iter->second->v[3] = w;
	}
}


