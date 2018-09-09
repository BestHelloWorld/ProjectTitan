#include "terrain.h"

#include "utils.h"
#include "program.h"
#include "model.h"
#include "camera.h"

Terrain::Terrain() :mHeightMapContent(0)
{
	mPosition = new cm::vec3[TERRAIN_SIZE*TERRAIN_SIZE];
	mVertices = new VertexBuffer;
	mProgram = new Program;

	mModelMatrix = new cm::mat4(1.0f);
}

Terrain::~Terrain()
{
	delete mModelMatrix;
}

void Terrain::Init(const CHAR * vs, const CHAR * fs, Camera * camera, const CHAR * path)
{
	if ((!vs) || (!fs) || (!camera))
		return;
	mCamera = camera;
	mProgram->Init(vs, fs, mCamera);
	mProgram->SetUniform4f(SHADER_MATERIAL_AMBIENT, 1.0f, 1.0f, 1.0f, 1.0f);
	SetTexture(SHADER_SAMPLER2D_MAIN_TEXTURE, (UINT)0);
	//mProgram->SetVector4f(SHADER_MATERIAL_DIFFUSE, 0.5f, 0.5f, 0.5f, 1.0f);

	mVertices->SetSize(TERRAIN_SIZE*TERRAIN_SIZE * 6);

	SetHeightMap(path);

	if (mHeightMapContent == 0)
	{
		for (UINT i = 0; i < TERRAIN_SIZE; ++i)
		{
			for (UINT j = 0; j < TERRAIN_SIZE; ++j)
			{
				mPosition[i * TERRAIN_SIZE + j] = cm::vec3((FLOAT)(j*TERRAIN_QUAD_SIZE), 0.0f, (FLOAT)(i*TERRAIN_QUAD_SIZE));
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
					cm::vec3((FLOAT)(j*TERRAIN_QUAD_SIZE), _getHeight(i, j), (FLOAT)(i*TERRAIN_QUAD_SIZE));
			}
		}
	}

	INT index = 0;
	for (int i = 0; i < TERRAIN_SIZE - 1; ++i)
	{
		for (int j = 0; j < TERRAIN_SIZE - 1; ++j)
		{
			mVertices->SetPosition(index++, mPosition[i*TERRAIN_SIZE + j].x, mPosition[i*TERRAIN_SIZE + j].y, mPosition[i*TERRAIN_SIZE + j].z);
			mVertices->SetPosition(index++, mPosition[(i + 1)*TERRAIN_SIZE + j].x, mPosition[(i + 1)*TERRAIN_SIZE + j].y, mPosition[(i + 1)*TERRAIN_SIZE + j].z);
			mVertices->SetPosition(index++, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].x, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].y, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].z);
			mVertices->SetPosition(index++, mPosition[i*TERRAIN_SIZE + j].x, mPosition[i*TERRAIN_SIZE + j].y, mPosition[i*TERRAIN_SIZE + j].z);
			mVertices->SetPosition(index++, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].x, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].y, mPosition[(i + 1)*TERRAIN_SIZE + j + 1].z);
			mVertices->SetPosition(index++, mPosition[i*TERRAIN_SIZE + j + 1].x, mPosition[i*TERRAIN_SIZE + j + 1].y, mPosition[i*TERRAIN_SIZE + j + 1].z);

			cm::vec3 normal = _getNormal(i, j);
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

	mVertices->UpdateData();
	mVertices->Clear();

	InitVAO();

	mLightViewMatrix = NULL;
	mLightProjectMatrix = NULL;

	mAmbientColor = cm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	mDisffuseColor = cm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	mSpecularColor = cm::vec4(0.9f, 0.9f, 0.9f, 1.0f);

	mModelStack = new std::stack<cm::mat4*>;
}

void Terrain::InitVAO()
{
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);
	mVertices->Bind();
	INT location = -1;
	INT program = mProgram->GetProgramId();

	// VERTEX ATTRIB
	location = glGetAttribLocation(program, SHADER_ATTRIB_POSITION);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)((Vertex *)0)->Position);
	}
	location = glGetAttribLocation(program, SHADER_ATTRIB_TEXCOORD);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)((Vertex *)0)->Texcoord);
	}
	location = glGetAttribLocation(program, SHADER_ATTRIB_NORMAL);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)((Vertex *)0)->Normal);
	}
	location = glGetAttribLocation(program, SHADER_ATTRIB_COLOR);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)((Vertex *)0)->Color);
	}
	mVertices->Unbind();
	glBindVertexArray(0);
}

void Terrain::SetLightPos(FLOAT x, FLOAT y, FLOAT z, FLOAT w, BOOL specular)
{
	mProgram->SetUniform4f("U_LightPos", x, y, z, w);
	UpdateMaterial(specular);
}

void Terrain::SetAmbientMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mAmbientColor.r = r;
	mAmbientColor.g = g;
	mAmbientColor.b = b;
	mAmbientColor.a = a;
	UpdateMaterial(FALSE);
}

void Terrain::SetDisffuseMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mDisffuseColor.r = r;
	mDisffuseColor.g = g;
	mDisffuseColor.b = b;
	mDisffuseColor.a = a;
	UpdateMaterial(FALSE);
}

void Terrain::SetSpecularMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mSpecularColor.r = r;
	mSpecularColor.g = g;
	mSpecularColor.b = b;
	mSpecularColor.a = a;
	UpdateMaterial(TRUE);
}

void Terrain::UpdateMaterial(BOOL specular)
{
	mProgram->SetUniform4f(SHADER_MATERIAL_AMBIENT, mAmbientColor.r, mAmbientColor.g, mAmbientColor.b, mAmbientColor.a);
	mProgram->SetUniform4f(SHADER_MATERIAL_DIFFUSE, mDisffuseColor.r, mDisffuseColor.g, mDisffuseColor.b, mDisffuseColor.a);
	if (specular)
		mProgram->SetUniform4f(SHADER_MATERIAL_SPECULAR, mSpecularColor.r, mSpecularColor.g, mSpecularColor.b, mSpecularColor.a);
	else
		mProgram->SetUniform4f(SHADER_MATERIAL_SPECULAR, 0.0f, 0.0f, 0.0f, 1.0f);
}

void Terrain::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = cm::translate(x, y, z);
}

void Terrain::Move(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = *mModelMatrix * cm::translate(x, y, z);
}

Program * Terrain::GetProgram()
{
	return mProgram;
}

void Terrain::Draw(GLenum draw_type)
{
	mProgram->Bind();
	mProgram->SetUniform4f(SHADER_UNIFORM_CAMERA_POSITION, mCamera->mEye.x, mCamera->mEye.y, mCamera->mEye.z, 1.0f);

	INT program = mProgram->GetProgramId();
	INT location = -1;

	glBindVertexArray(mVertexArray);
	//mModelMatrix = *new esm::mat4(m);
	if (mProgram >= 0)
	{
		location = glGetUniformLocation(program, SHADER_UNIFORM_NORMAL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, cm::inverse(*mModelMatrix).v);
		}

		location = glGetUniformLocation(program, SHADER_UNIFORM_MODEL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, mModelMatrix->v);
		}

		location = glGetUniformLocation(program, SHADER_UNIFORM_LIGHT_VIEW_MATRIX);
		if (location >= 0 && mLightViewMatrix != 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, mLightViewMatrix);
		}

		location = glGetUniformLocation(program, SHADER_UNIFORM_LIGHT_PROJECTION_MATRIX);
		if (location >= 0 && mLightProjectMatrix != 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, mLightProjectMatrix);
		}
	}
	glDrawArrays(draw_type, 0, mVertices->mVertexesCount);

	glBindVertexArray(0);
	mProgram->Unbind();
}

void Terrain::Release()
{
	mProgram->Release();
	delete mProgram;

	for (UINT i = 0; i < mModelStack->size(); ++i)
	{
		delete mModelStack->top();
		mModelStack->pop();
	}
	delete mModelStack;
}


void Terrain::ShadowDraw()
{
	mVertices->Bind();

	INT program = mProgram->GetProgramId();
	INT location = -1;
	location = glGetAttribLocation(program, SHADER_ATTRIB_POSITION);

	glBindVertexArray(mVertexArray);

	if (mProgram >= 0)
	{
		location = glGetUniformLocation(program, SHADER_UNIFORM_MODEL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, mModelMatrix->v);
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, mVertices->mVertexesCount);

	glBindVertexArray(0);
	mVertices->Unbind();
}

void Terrain::SetTexture(const CHAR * name, const CHAR * texture)
{
	mProgram->SetTexture(name, texture);
}

void Terrain::SetTexture(const CHAR * name, UINT texture)
{
	mProgram->SetTexture(name, texture);
}

Terrain * Terrain::Clone()
{
	Terrain * terr = new Terrain;
	memcpy(terr, this, sizeof(Terrain));
	terr->mModelMatrix = new cm::mat4(1.0f);
	memcpy(terr->mModelMatrix, mModelMatrix, sizeof(cm::mat4));
	return terr;
}

void Terrain::SetShadowCamera(Camera camera)
{
	mLightProjectMatrix = camera.GetProjectionMatrix();
	mLightViewMatrix = camera.GetViewMatrix();
}

void Terrain::SetShadowMap(UINT shadowMap)
{
	mProgram->SetTexture(SHADER_SAMPLER2D_SHADOW_TEXTURE, shadowMap);
}

void Terrain::PushMatrix()
{
	cm::mat4 *c = new cm::mat4;
	memcpy(c, mModelMatrix, sizeof(cm::mat4));
	mModelStack->push(c);
}

void Terrain::PopMatrix()
{
	if (mModelStack->size() <= 0)
		return;
	cm::mat4 *r = mModelStack->top();
	memcpy(mModelMatrix, r, sizeof(cm::mat4));
	mModelStack->pop();
	delete r;
}

void Terrain::SetFogLimit(FLOAT nNear, FLOAT nFar)
{
	mProgram->SetFogLimit(nNear, nFar);
}

void Terrain::SetFogColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mProgram->SetFogColor(r, g, b, a);
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
	*mModelMatrix = *mModelMatrix * cm::scale(x, y, z);
}

void Terrain::SetUniform4f(const CHAR * uniform, FLOAT x, FLOAT y, FLOAT z, FLOAT w)
{
	mProgram->SetUniform4f(uniform, x, y, z, w);
}

FLOAT Terrain::_getHeight(INT i, INT j)
{

	if (mHeightMapContent == 0)
		return 0.0f;
	i = i < 0 ? 0 : i;
	j = j < 0 ? 0 : j;
	return mHeightMapContent[(i * mHeightMapWidth + j) * 3] / 255.0f;
}

cm::vec3 Terrain::_getNormal(INT i, INT j)
{
	FLOAT l = _getHeight(i, j - 1);
	FLOAT r = _getHeight(i, j + 1);
	FLOAT f = _getHeight(i - 1, j);
	FLOAT b = _getHeight(i + 1, j);
	return cm::vec3(l - r, 2.0f, f - b).normalize();
}
