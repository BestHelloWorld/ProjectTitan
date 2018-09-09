#include "model.h"

#include "utils.h"
#include "camera.h"
#include "program.h"


//------------------------------------------------------------
// VertexBuffer
//------------------------------------------------------------
UINT VertexBuffer::CreateVertexBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *data)
{
	UINT buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(bufferType, buffer);
	glBufferData(bufferType, size, data, usage);
	glBindBuffer(bufferType, 0);
	return buffer;
}

void VertexBuffer::ReleaseVertexBufferObject(UINT vbo)
{
	glDeleteBuffers(1, &vbo);
}


VertexBuffer::VertexBuffer()
{
	mVBO = -1;
}

VertexBuffer::~VertexBuffer()
{
	Clear();
}

void VertexBuffer::Release()
{
	Clear();
	if (mVBO != -1)
		glDeleteBuffers(1, &mVBO);
}

void VertexBuffer::SetSize(INT vertexesCount)
{
	mVertexesCount = vertexesCount;
	mVertexes = new Vertex[mVertexesCount];
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
}
void VertexBuffer::UpdateData()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mVertexesCount, mVertexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Clear()
{
	if(mVertexes != NULL)
	{
		delete[] mVertexes;
		mVertexes = NULL;
	}
}

UINT VertexBuffer::GetVBO()
{
	return mVBO;
}

Vertex *VertexBuffer::Get(INT index)
{
	return &(mVertexes[index]);
}


//------------------------------------------------------------
// Model
//------------------------------------------------------------
Model::Model()
{
	mIsHide = FALSE;
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

	mVertexBuffer->UpdateData();
	mVertexBuffer->Clear();

	mProgram = program;
	mMoveAdded = cm::vec3(0.0f);
	mTime = 0.0f;

	mModelMatrix = new cm::mat4(1.0f);
	mModelStack = new std::stack<cm::mat4*>;

	LOG_D("load model finish, vertexes count %d", vertexCount);

	mLightViewMatrix = NULL;
	mLightProjectMatrix = NULL;

	InitVAO();
}

void Model::SetShadowCamera(Camera camera)
{
	mLightProjectMatrix = camera.GetProjectionMatrix();
	mLightViewMatrix = camera.GetViewMatrix();
}

void Model::InitVAO()
{
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);
	mVertexBuffer->Bind();
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

	mVertexBuffer->Unbind();
	glBindVertexArray(0);
}

void Model::Release()
{
	for (UINT i = 0; i < mModelStack->size(); ++i)
	{
		delete mModelStack->top();
		mModelStack->pop();
	}

	CloneRelease();
	glDeleteVertexArrays(1, &mVertexArray);
	glDeleteBuffers(1, &mVertexBuffer->mVBO);
}

void Model::Draw()
{
	if (mIsHide)
		return;

	glBindVertexArray(mVertexArray);

	INT location = -1;
	if (mProgram >= 0)
	{
		location = glGetUniformLocation(mProgram, SHADER_UNIFORM_NORMAL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, cm::inverse(*mModelMatrix).v);
		}

		location = glGetUniformLocation(mProgram, SHADER_UNIFORM_MODEL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, mModelMatrix->v);
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

	glBindVertexArray(0);
}

void Model::Move(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = *mModelMatrix * cm::translate(mMoveAdded.x + x, mMoveAdded.y + y, mMoveAdded.z + z);

	//LOG_D("move add x : %f, y : %f, z : %f", mMoveAdded.x, mMoveAdded.y, mMoveAdded.z);
}

void Model::SetMoveAdd(FLOAT x, FLOAT y, FLOAT z)
{
	mMoveAdded.x = x;
	mMoveAdded.y = y;
	mMoveAdded.z = z;
}

void Model::Scale(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = *mModelMatrix * cm::scale(x, y, z);
}

void Model::Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = *mModelMatrix * cm::rotate(angle, x, y, z);
}

void Model::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	*mModelMatrix = cm::translate(x, y, z);
}

cm::vec3 Model::GetPosition()
{
	FLOAT x = (*mModelMatrix)[3];
	FLOAT y = (*mModelMatrix)[7];
	FLOAT z = (*mModelMatrix)[11];
	return cm::vec3(x, y, z);
}

void Model::MultiplyModelMatrix(cm::mat4 m)
{
	*mModelMatrix = *mModelMatrix * m;
}

void Model::LoadIdentity()
{
	*mModelMatrix = cm::mat4(1.0f);
}

void Model::PushMatrix()
{
	mModelStack->push(new cm::mat4(*mModelMatrix));
}

void Model::PopMatrix()
{
	if (mModelStack->size() <= 0)
		return;

	cm::mat4 * tm = mModelStack->top();
	memcpy(mModelMatrix, tm, sizeof(cm::mat4));
	mModelStack->pop();
	delete tm;
}

Model * Model::Clone()
{
	Model * m = new Model;
	memcpy(m, this, sizeof(Model));
	m->mModelMatrix = new cm::mat4;
	*(m->mModelMatrix) = *mModelMatrix;
	m->mTime = 0.0f;
	m->mIsHide = FALSE;
	m->mModelStack = new std::stack<cm::mat4*>;
	return m;
}

void Model::CloneRelease()
{
	delete mModelStack;
	delete mModelMatrix;
	//delete this;
}

void Model::Hide()
{
	mIsHide = TRUE;
}


//------------------------------------------------------------
// ModelEmit
//------------------------------------------------------------
ModelEmit::ModelEmit()
{
	mSampleModel = NULL;
	mProgram = NULL;
	mCallback = NULL;
}

ModelEmit::~ModelEmit()
{
	delete mSampleModel;
	for (UINT i = 0; i < mModels.size(); ++i)
	{
		delete mModels[i];
	}
}


void ModelEmit::Init(const CHAR * vs, const CHAR * fs, Camera * camera, const CHAR* modelPath, cm::vec3 emitDir, cm::vec3 emitPos, FLOAT speed, FLOAT time)
{
	mProgram = new Program;
	mProgram->Init(vs, fs, camera);
	mProgram->SetUniform4f(SHADER_MATERIAL_AMBIENT, 1.0f, 1.0f, 1.0f, 1.0f);

	mSampleModel = new Model;
	mSampleModel->Init(mProgram->GetProgramId(), modelPath);
	mSampleModel->SetPosition(emitPos.x, emitPos.y, emitPos.z);

	mEmitDir = emitDir;
	mEmitPos = emitPos;
	mSpeed = speed;
	mTime = time;
}

void ModelEmit::Erase(Model * model)
{
	for (UINT i = 0; i < mModels.size(); ++i)
	{
		if (mModels[i] == model)
		{
			model->CloneRelease();
			mModels.erase(mModels.begin() + i);

			delete mModelsDir[i];
			mModelsDir.erase(mModelsDir.begin() + i);
			break;
		}
	}

	delete model;
}

void ModelEmit::Clear()
{
	for (UINT i = 0; i < mModels.size(); ++i)
	{
		mModels[i]->CloneRelease();
	}
	mModels.clear();
}

void ModelEmit::Update(FLOAT second)
{
	for (UINT i = 0; i < mModels.size(); ++i)
	{
		mModels[i]->mTime += second;
		//LOG_D("models : %d , time : %f", mModels.size(), mModels[i]->mTime);
		if (mModels[i]->mTime >= mTime)
		{
			//mModels[i]->CloneRelease();
			//delete mModels[i];
			//mModels.erase(mModels.begin() + i);

			//delete mModelsDir[i];
			//mModelsDir.erase(mModelsDir.begin() + i);

			Erase(mModels[i]);

			continue;
		}

		cm::vec3 * dir = mModelsDir[i];
		cm::vec3 pos = *dir * mSpeed * second;
		mModels[i]->Move(pos.x, pos.y, pos.z);
		if (mCallback)
		{
			mCallback(mModels[i]);
		}
	}

}

void ModelEmit::SetCallback(ModelEmitCallback func)
{
	if (func == NULL)return;
	mCallback = func;
}

void ModelEmit::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	mEmitPos.x = x;
	mEmitPos.y = y;
	mEmitPos.z = z;
	mSampleModel->SetPosition(x, y, z);
}

void ModelEmit::SetPosition(cm::vec3 pos)
{
	SetPosition(pos.x, pos.y, pos.z);
}

void ModelEmit::SetEmitDirection(FLOAT x, FLOAT y, FLOAT z)
{
	mEmitDir.x = x;
	mEmitDir.y = y;
	mEmitDir.z = z;
	mEmitDir = mEmitDir.normalize();
}

void ModelEmit::SetLightPosition(FLOAT x, FLOAT y, FLOAT z, BOOL specular)
{
	mProgram->SetUniform4f(SHADER_MATERIAL_AMBIENT, 0.2f, 0.2f, 0.2f, 1.0f);
	mProgram->SetUniform4f(SHADER_MATERIAL_DIFFUSE, 0.6f, 0.6f, 0.6f, 1.0f);
	if(specular)
		mProgram->SetUniform4f(SHADER_MATERIAL_AMBIENT, 0.0f, 0.0f, 0.0f, 1.0f);
	else
		mProgram->SetUniform4f(SHADER_MATERIAL_AMBIENT, 1.0f, 1.0f, 1.0f, 1.0f);
	mProgram->SetUniform4f(SHADER_UNIFORM_LIGHT_POSITION, x, y, z, 0.0f);
}

void ModelEmit::Move(FLOAT x, FLOAT y, FLOAT z)
{
	mSampleModel->Move(x, y, z);
}

void ModelEmit::Scale(FLOAT x, FLOAT y, FLOAT z)
{
	mSampleModel->Scale(x, y, z);
}

void ModelEmit::Rotate(FLOAT angle, FLOAT x, FLOAT y, FLOAT z)
{
	mSampleModel->Rotate(angle, x, y, z);
}

void ModelEmit::SetTexture(const CHAR * name, const CHAR * filePath)
{
	mProgram->SetTexture(name, filePath);
}

void ModelEmit::SetTexture(const CHAR * name, UINT texture)
{
	mProgram->SetTexture(name, (UINT)texture);
}

Model * ModelEmit::GetSampleModel() const
{
	return mSampleModel;
}

void ModelEmit::Emit()
{
	Model * m = mSampleModel->Clone();
	mModels.push_back(m);

	cm::vec3 * dir = new cm::vec3(mEmitDir);
	mModelsDir.push_back(dir);
}

void ModelEmit::Emit(FLOAT x, FLOAT y, FLOAT z)
{
	Model * m = mSampleModel->Clone();
	m->MultiplyModelMatrix(cm::translate(x, y, z));
	mModels.push_back(m);

	cm::vec3 * dir = new cm::vec3(mEmitDir);
	mModelsDir.push_back(dir);
}

void ModelEmit::Draw()
{
	cm::vec3 pos = mProgram->GetCamera()->mEye;
	mProgram->SetUniform4f(SHADER_UNIFORM_CAMERA_POSITION, pos.x, pos.y, pos.z, 0.0f);
	mProgram->Bind();
	for (UINT i = 0; i < mModels.size(); ++i)
	{
		mModels[i]->Draw();
	}
	mProgram->Unbind();
}


void ModelEmit::ShadowDraw()
{
	for (UINT i = 0; i < mModels.size(); ++i)
	{
		mModels[i]->Draw();
	}
}
