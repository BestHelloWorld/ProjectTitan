#include "model.h"

#include "utils.h"
#include "camera.h"

UINT VertexBuffer::CreateVertexBufferObject(ENUM bufferType, GLsizeiptr size, ENUM usage, void *data)
{
	UINT buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(bufferType, buffer);
	glBufferData(bufferType, size, data, usage);
	glBindBuffer(bufferType, 0);
	return buffer;
}

void VertexBuffer::SetSize(INT vertexesCount)
{
	mVertexesCount = vertexesCount;
	mVertexes = new Vertex[mVertexesCount]{ 0 };
	mVBO = CreateVertexBufferObject(GL_ARRAY_BUFFER, sizeof(Vertex)*mVertexesCount, GL_STATIC_DRAW);
}

void VertexBuffer::SetPosition(INT index, FLOAT x, FLOAT y, FLOAT z, FLOAT w)
{
	mVertexes[index].Position[0] = x;
	mVertexes[index].Position[1] = y;
	mVertexes[index].Position[2] = z;
	mVertexes[index].Position[3] = w;
}

void VertexBuffer::SetColor(INT index, FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mVertexes[index].Color[0] = r;
	mVertexes[index].Color[1] = g;
	mVertexes[index].Color[2] = b;
	mVertexes[index].Color[3] = a;
}

void VertexBuffer::SetTexcoord(INT index, FLOAT x, FLOAT y)
{
	mVertexes[index].Texcoord[0] = x;
	mVertexes[index].Texcoord[1] = y;
}

void VertexBuffer::SetNormal(INT index, FLOAT x, FLOAT y, FLOAT z)
{
	mVertexes[index].Normal[0] = x;
	mVertexes[index].Normal[1] = y;
	mVertexes[index].Normal[2] = z;
	mVertexes[index].Normal[3] = 1.0f;
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mVertexesCount, mVertexes, GL_STATIC_DRAW);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Vertex *VertexBuffer::Get(INT index)
{
	return &(mVertexes[index]);
}


Model::Model() :mModelMatrix(0)
{
	mModelMatrix = new esm::mat4(1.0f);
}

void Model::Init(const INT program, const CHAR *modelPath)
{
	struct FloatData
	{
		FLOAT v[3];
	};
	struct VertexDefine
	{
		INT posIndex;
		INT texcoordIndex;
		INT normalIndex;
	};
	INT nFileSize = 0;
	UCHAR *fileContent = LoadFileContent(modelPath, nFileSize);
	if (fileContent == nullptr)
	{
		LOG_E("Load file content error : line %d", __LINE__);
		return;
	}
	std::vector<FloatData> positions, texcoords, normals;
	std::vector<VertexDefine> vertexes;
	std::stringstream ssFileContent((CHAR *)fileContent);
	std::string temp;
	CHAR szOneLine[256];
	while (!ssFileContent.eof())
	{
		memset(szOneLine, 0, 256);
		ssFileContent.getline(szOneLine, 256);
		if (strlen(szOneLine) > 0)
		{
			if (szOneLine[0] == 'v')
			{
				std::stringstream ssOneLine(szOneLine);
				if (szOneLine[1] == 't')
				{
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					texcoords.push_back(floatData);
				}
				else if (szOneLine[1] == 'n')
				{
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					ssOneLine >> floatData.v[2];
					normals.push_back(floatData);
				}
				else
				{
					ssOneLine >> temp;
					FloatData floatData;
					ssOneLine >> floatData.v[0];
					ssOneLine >> floatData.v[1];
					ssOneLine >> floatData.v[2];
					positions.push_back(floatData);
				}
			}
			else if (szOneLine[0] == 'f')
			{
				std::stringstream ssOneLine(szOneLine);
				ssOneLine >> temp;
				std::string vertexStr;
				for (int i = 0; i < 3; i++)
				{
					ssOneLine >> vertexStr;
					size_t pos = vertexStr.find_first_of('/');
					std::string posIndexStr = vertexStr.substr(0, pos);
					size_t pos2 = vertexStr.find_first_of('/', pos + 1);
					std::string texcoordIndexStr = vertexStr.substr(pos + 1, pos2 - 1 - pos);
					std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);
					VertexDefine vd;
					vd.posIndex = atoi(posIndexStr.c_str());
					vd.texcoordIndex = atoi(texcoordIndexStr.c_str());
					vd.normalIndex = atoi(normalIndexStr.c_str());
					vertexes.push_back(vd);
				}
			}
		}
	}
	delete fileContent;
	INT vertexCount = (int)vertexes.size();
	mVertexBuffer = new VertexBuffer;
	mVertexBuffer->SetSize(vertexCount);
	for (int i = 0; i < vertexCount; ++i)
	{
		float *temp = positions[vertexes[i].posIndex - 1].v;
		mVertexBuffer->SetPosition(i, temp[0], temp[1], temp[2]);
		temp = texcoords[vertexes[i].texcoordIndex - 1].v;
		mVertexBuffer->SetTexcoord(i, temp[0], temp[1]);
		temp = normals[vertexes[i].normalIndex - 1].v;
		mVertexBuffer->SetNormal(i, temp[0], temp[1], temp[2]);
	}

	mProgram = program;
	LOG_D("load model finish, vertexes count %d", vertexCount);
}

void Model::SetShadowCamera(Camera camera)
{
	mLightProjectMatrix = camera.GetProjectionMatrix();
	mLightViewMatrix = camera.GetViewMatrix();
}

void Model::Draw()
{
	mVertexBuffer->Bind();

	GLint location = -1;

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

	//mModelMatrix = *new esm::mat4(m);
	if (mProgram >= 0)
	{
		FLOAT * nm = esm::value_ptr(esm::inverse(esm::mat4(*mModelMatrix)));

		location = glGetUniformLocation(mProgram, SHADER_UNIFORM_NORMAL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, (nm));
		}

		location = glGetUniformLocation(mProgram, SHADER_UNIFORM_MODEL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, esm::value_ptr(*mModelMatrix));
		}

		location = glGetUniformLocation(mProgram, SHADER_UNIFORM_LIGHT_VIEW_MATRIX);
		if (location >= 0 && mLightViewMatrix != 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, mLightViewMatrix);
		}

		location = glGetUniformLocation(mProgram, SHADER_UNIFORM_LIGHT_PROJECTION_MATRIX);
		if (location >= 0 && mLightProjectMatrix != 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, mLightProjectMatrix);
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer->mVertexesCount);
	mVertexBuffer->Unbind();
}

void Model::Move(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = *mModelMatrix * esm::translate(x, y, z);
}

void Model::SetSize(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = *mModelMatrix * esm::scale(x, y, z);
}

void Model::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = esm::translate(x, y, z);
}

void Model::MultiplyModelMatrix(esm::mat4 m)
{
	esm::mat4 r(*mModelMatrix);
	r = r * m;
	*mModelMatrix = (r);
}

void Model::LoadIdentity()
{
	*mModelMatrix = (esm::mat4(1.0f));
}
