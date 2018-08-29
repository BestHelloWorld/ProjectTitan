#include "pointsprite.h"

#include "camera.h"
#include "program.h"


//------------------------------------------------------------
// PointSprite
//------------------------------------------------------------
PointSprite::PointSprite()
{
}

PointSprite::~PointSprite()
{
	delete mPosition;
	delete mColor;
	delete mMoveDir;
}

void PointSprite::Init(INT program, cm::vec3 pos, cm::vec3 moveDir)
{
	mProgram = program;
	//mPointSize = pointSize;
	mPosition = new cm::vec3(pos);
	mMoveDir = new cm::vec3(moveDir.normalize());
	mColor = new cm::vec4(1.0f);
	mTime = 0.0f;
}

void PointSprite::SetColor(FLOAT r, FLOAT g, FLOAT b)
{
	mColor->r = r;
	mColor->g = g;
	mColor->b = b;
}

void PointSprite::Move(FLOAT x, FLOAT y, FLOAT z)
{
	*mPosition = *mPosition + cm::vec3(x, y, z);
}

void PointSprite::Move(cm::vec3 pos)
{
	*mPosition = *mPosition + pos;
}

void PointSprite::Move(FLOAT distance)
{
	*mPosition = *mPosition + (*mMoveDir * distance);
}

void PointSprite::Draw()
{
	INT location = -1;
	if (mProgram >= 0)
	{
		location = glGetAttribLocation(mProgram, SHADER_ATTRIB_POSITION);
		if (location >= 0)
		{
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(FLOAT) * 3, (void *)mPosition->v);
		}
		location = glGetAttribLocation(mProgram, SHADER_ATTRIB_COLOR);
		if (location >= 0)
		{
			*mColor = *mColor * 1.0f - (mTime / POINT_SPRITE_TIME);
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(FLOAT) * 4,
			                      (void *)(*mColor).v);
		}

		location = glGetUniformLocation(mProgram, SHADER_UNIFORM_MODEL_MATRIX);
		if (location >= 0)
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, cm::value_ptr(cm::mat4(1.0f)));
		}
	}
	glDrawArrays(GL_POINTS, 0, 1);
}


//------------------------------------------------------------
// SpriteEmit
//------------------------------------------------------------
void SpriteEmit::Init(const CHAR* vs, const CHAR* fs, cm::vec3 emitDir, Camera* cam,
                      FLOAT newSpritePerSecond, FLOAT spriteSpeed, BOOL autoEmit)
{
	mProgram = new Program;
	mProgram->Init(vs, fs, cam);
	mEmitDir = new cm::vec3(emitDir.normalize());
	mEmitPos = new cm::vec3(0.0f);

	mNewSpritePerSecond = newSpritePerSecond;
	mSpriteSpeed = spriteSpeed;
	mAutoEmit = autoEmit;

	//glEnable(GL_POINT_SPRITE);

	//LOG_D("%d \n", sizeof(PointSprite));
}

void SpriteEmit::Update(FLOAT second)
{
	//mCurrentTime += second;
	//mCurrentTime = mCurrentTime >= 1.0f ? 0.0f : mCurrentTime;
	//INT count = static_cast<INT>(mCurrentTime / mNewSpritePerSecond + 0.5f);
	std::vector<PointSprite*>::iterator iter;
	for (UINT i = 0; i < mSprites.size(); ++i)
	{
		mSprites[i]->mTime += second;

		if (mSprites[i]->mTime > POINT_SPRITE_TIME)
		{
			iter = mSprites.begin() + i;
			delete mSprites[i];
			mSprites.erase(iter);
			i--;
			continue;
		}

		//esm::vec3 pos = mEmitDir * mSpriteSpeed * second;
		mSprites[i]->Move(mSpriteSpeed * second);
	}

	//LOG_D("sprite size : %d", mSprites.size());

	PointSprite* sprite = NULL;
	cm::vec3 pos;
	//for (INT i = 0; i < count; ++i)
	if (mAutoEmit)
	{
		Emit();
	}
}

void SpriteEmit::Emit()
{
	PointSprite* sprite = new PointSprite;
	sprite->Init(mProgram->GetProgramId(), *mEmitPos, *mEmitDir);
	//LOG_D("%s\n", mEmitPos->dump());
	//sprite->Move(mEmitPos);
	//esm::vec3 pos = mEmitDir * mSpriteSpeed * (count - i) * (second / mNewSpritePerSecond);
	//mSprites[i]->Move(pos);

	mSprites.push_back(sprite);
}

void SpriteEmit::SetPosition(cm::vec3 pos)
{
	*mEmitPos = pos;
}

void SpriteEmit::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	mEmitPos->x = x;
	mEmitPos->y = y;
	mEmitPos->z = z;
}

void SpriteEmit::Draw()
{
	//glPointSize(POINT_SPRITE_SIZE);
	//glEnable(GL_POINT_SPRITE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	mProgram->Bind();
	for (UINT i = 0; i < mSprites.size(); ++i)
	{
		mSprites[i]->Draw();
	}
	mProgram->Unbind();


	glDisable(GL_BLEND);
	//glDisable(GL_POINT_SPRITE);
	glEnable(GL_DEPTH_TEST);
}
