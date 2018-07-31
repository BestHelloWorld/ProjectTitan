#include "terrain.h"

#include "utils.h"
#include "program.h"
#include "model.h"
#include "camera.h"

Terrain::Terrain():mHeightMapContent(0)
{
	mPosition = new esm::vec3[TERRAIN_SIZE*TERRAIN_SIZE];
	mVertices = new VertexBuffer;
	
	mModelMatrix = new esm::mat4(1.0f);
}

Terrain::~Terrain()
{
	delete mPosition;
	delete mVertices;
}

void Terrain::Init(const INT program, const CHAR * path)
{
	mProgram = program;
	mVertices->SetSize(TERRAIN_SIZE*TERRAIN_SIZE*6);

	SetHeightMap(path);

	if (mHeightMapContent == 0)
	{
		for (int i = 0; i < TERRAIN_SIZE; ++i)
		{
			for (int j = 0; j < TERRAIN_SIZE; ++j)
			{
				mPosition[i * TERRAIN_SIZE + j] = esm::vec3(j*TERRAIN_QUAD_SIZE, 0.0f, i*TERRAIN_QUAD_SIZE);
			}
		}
	}
	else
	{
		for (int i = 0; i < TERRAIN_SIZE; ++i)
		{
			for (int j = 0; j < TERRAIN_SIZE; ++j)
			{
				mPosition[i * TERRAIN_SIZE + j] =
					esm::vec3(j*TERRAIN_QUAD_SIZE, _getHeight(i, j), i*TERRAIN_QUAD_SIZE);
			}
		}
	}

	INT index = 0;
	for (int i = 0; i < TERRAIN_SIZE -1; ++i)
	{
		for (int j = 0; j < TERRAIN_SIZE - 1; ++j)
		{
			mVertices->SetPosition(index++, mPosition[i*TERRAIN_SIZE + j].x, mPosition[i*TERRAIN_SIZE + j].y, mPosition[i*TERRAIN_SIZE + j].z);
			mVertices->SetPosition(index++, mPosition[(i + 1)*TERRAIN_SIZE + j].x, mPosition[(i + 1)*TERRAIN_SIZE + j].y, mPosition[(i + 1)*TERRAIN_SIZE + j].z);
			mVertices->SetPosition(index++, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].x, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].y, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].z);
			mVertices->SetPosition(index++, mPosition[i*TERRAIN_SIZE + j].x, mPosition[i*TERRAIN_SIZE + j].y, mPosition[i*TERRAIN_SIZE + j].z);
			mVertices->SetPosition(index++, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].x, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].y, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].z);
			mVertices->SetPosition(index++, mPosition[i*TERRAIN_SIZE + j + 1].x, mPosition[i*TERRAIN_SIZE + j + 1].y, mPosition[i*TERRAIN_SIZE + j + 1].z);

			esm::vec3 normal = _getNormal(i, j);
			mVertices->SetNormal(index - 6, normal.x, normal.y, normal.z);
			normal = _getNormal(i + 1, j);
			mVertices->SetNormal(index - 5, normal.x, normal.y, normal.z);
			normal = _getNormal(i + 1, j + 1);
			mVertices->SetNormal(index - 4, normal.x, normal.y, normal.z);
			normal = _getNormal(i, j);
			mVertices->SetNormal(index - 3, normal.x, normal.y, normal.z);
			normal = _getNormal(i + 1, j + 1);
			mVertices->SetNormal(index - 2, normal.x, normal.y, normal.z);
			normal = _getNormal(i, j + 1);
			mVertices->SetNormal(index - 1, normal.x, normal.y, normal.z);

			mVertices->SetTexcoord(index - 6, 0.0f, 1.0f);
			mVertices->SetTexcoord(index - 5, 0.0f, 0.0f);
			mVertices->SetTexcoord(index - 4, 1.0f, 0.0f);
			mVertices->SetTexcoord(index - 3, 0.0f, 1.0f);
			mVertices->SetTexcoord(index - 2, 1.0f, 0.0f);
			mVertices->SetTexcoord(index - 1, 1.0f, 1.0f);
		}
	}

	index;
}


void Terrain::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = *mModelMatrix * esm::translate(x, y, z);
}

void Terrain::Draw()
{
	mVertices->Bind();

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
	glDrawArrays(GL_TRIANGLES, 0, mVertices->mVertexesCount);
	mVertices->Unbind();
}

void Terrain::SetShadowCamera(Camera camera)
{
	mLightProjectMatrix = camera.GetProjectionMatrix();
	mLightViewMatrix = camera.GetViewMatrix();
}

void Terrain::SetHeightMap(const CHAR * path)
{
	INT len;
	if (path == 0)
		return;
	mHeightMapContent = LoadFileContent(path, len);
	if (len <= 0)
	{
		mHeightMapContent = 0;
		return;
	}

	mHeightMapContent = DecodeBMP(mHeightMapContent, mHeightMapWidth, mHeightMapHeight);
	if (mHeightMapWidth <= 0 || mHeightMapHeight <= 0)
	{
		mHeightMapContent = 0;
		return;
	}
}

void Terrain::SetSize(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = *mModelMatrix * esm::scale(x, y, z);
}

FLOAT Terrain::_getHeight(INT i, INT j)
{
	if (mHeightMapContent == 0)
		return 0.0f;
	i = i < 0 ? 0 : i;
	j = j < 0 ? 0 : j;
	return mHeightMapContent[(i * mHeightMapWidth + j) * 3] / 255.0f;
}

esm::vec3 Terrain::_getNormal(INT i, INT j)
{
	FLOAT l = _getHeight(i, j - 1);
	FLOAT r = _getHeight(i, j + 1);
	FLOAT f = _getHeight(i - 1, j);
	FLOAT b = _getHeight(i + 1, j);
	return esm::vec3(l - r, 2.0f, f - b).normalize();
}
