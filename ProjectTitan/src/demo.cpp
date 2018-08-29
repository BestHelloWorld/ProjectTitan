#include "demo.h"

#include "core/framebuffer.h"
#include "core/animation.h"
#include "core/sound.h"

#include <thread>

Unit Raven;

// MODEL EMIT
ModelEmit Bullet;
ModelEmit Enemy;

INT score;
FLOAT textTime;
cm::vec4 textPos;

Terrain terrain;
Animation fall_anim;
Aircraft raven;

Program spriteProg;
ImageSprite imgSprite;
ButtonManager sm;

Camera testCam;

FrameBuffer fbo;

Sound sound;
UINT thud;

BOOL isGameover = FALSE;
BOOL isEnemy = FALSE;

void
BulletProc(Model * model)
{
	if (model->mIsHide)
		return;
	for (UINT i = 0; i < Enemy.mModels.size(); ++i)
	{
		//esm::vec3 pos1 = Raven.GetPosition();
		cm::vec3 pos1 = (*Enemy.mModels[i]).GetPosition();
		cm::vec3 pos2 = model->GetPosition();
		FLOAT distance = ((pos1 - pos2) * 0.5f).magnitude();
		if (distance <= 1.0f)
		{
			textPos = Bullet.mProgram->mCamera->WorldToScreen(pos1);
			textPos = textPos / textPos.w;
			
			model->Hide();
			Enemy.mModels[i]->SetMoveAdd(0.0f, -0.1f, 0.0f);
			textTime = 0.0f;

			score += 100;
		}
	}
}

void 
EnemyProc(Model * model)
{
	cm::vec3 raven_pos = raven.GetPosition();
	cm::vec3 model_pos = model->GetPosition();

	FLOAT distance = (raven_pos - model_pos).magnitude();
	if (distance <= 5.0f)
	{
		//fall_anim.Init(raven_pos, cm::vec4(raven_pos.x, 0.0f, raven_pos.z));
		//fall_anim.Set(&raven);
		isGameover = TRUE;
	}
}

void
MainScene::Init()
{
	Scene::Init();

	LOG_D("%s", (const CHAR*)glGetString(GL_VERSION));

	bMoveForward = FALSE, bMoveBackward = FALSE, bMoveLeft = FALSE, bMoveRight =
		FALSE, bMoveUp = FALSE, bMoveDown = FALSE;

	cam.Init(cm::vec3(0.0f, -10.0f, 0.0f), cm::vec3(0.0f, -10.0f, -1.0f),
		cm::vec3(0.0f, 1.0f, 0.0f));
	cam.CameraForward(-30.0f);

	shadowCam.Init(cm::vec3(0.0f, 50.0f, 0.0f), cm::vec3(0.0f, 0.0f, 0.0f),
		cm::vec3(0.0f, 0.0f, -1.0f));

	skybox.Init("res/shader/skybox.vs", "res/shader/skybox.fs", &cam,
		"res/images/skybox/front.bmp", "res/images/skybox/back.bmp",
		"res/images/skybox/left.bmp", "res/images/skybox/right.bmp",
		"res/images/skybox/top.bmp", "res/images/skybox/bottom.bmp");
	skybox.SetSize(10.0f);
	skybox.SetColor(0.5f, 0.5f, 0.5f);

	//spriteProg.Init("res/shader/v.vs", "res/shader/f.fs", &cam);
	//imgSprite.Init(spriteProg.GetProgramId(), "res/images/earth.bmp", 0.0f, 0.0f, 100.0f, 100.0f);

	fsq.Init(RIGHT_TOP_SCREEN);
	fsqProg.Init("res/shader/fullscreenquad.vs", "res/shader/fullscreenquad.fs",
		&cam);

	//TERRAIN INIT
	terrain.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &cam,
		"res/images/height_256x256.bmp");
	terrain.SetTexture(SHADER_SAMPLER2D_MAIN_TEXTURE, "res/images/grass.bmp");
	terrain.SetLightPos(0.0f, 10.0f, 0.0f, 0.0f);
	*terrain.mModelMatrix = *terrain.mModelMatrix
		* cm::rotate(45.0f, 0.0f, 1.0f, 0.0f);
	terrain.Move(-100.0f, -20.0f, -100.0f);
	terrain.SetSize(5.0f, 50.0f, 5.0f);
	terrain.SetSpecularMaterial(0.3f, 0.3f, 0.3f);
	terrain.SetFogLimit(50.0f, 400.0f);
	terrain.SetFogColor(0.0f, 0.0f, 0.0f);

	//MODEL INIT
	raven.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs",
		"res/objs/raven.obj", &cam);
	raven.SetLightPos(0.0f, 1.0f, 0.0f);
	raven.SetTexture("U_Texture", "res/images/raven.bmp");
	raven.BindCamera(&cam);
	raven.Move(0.0f, 20.0f, 0.0f);

	Bullet.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &cam,
		"res/objs/rocket.obj", cm::vec3(0.0f, 0.0f, 1.0f),
		raven.GetPosition(), 100.0f);
	//Bullet.Scale(0.2f, 0.2f, 0.2f);
	Bullet.SetLightPosition(0.0f, 1.0f, 0.0f);
	Bullet.SetCallback(BulletProc);

	cm::vec3 pos = raven.GetPosition();
	pos.z -= 300.0f;
	Enemy.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &cam,
		"res/objs/hunter/sources/hunter_rearside.obj",
		cm::vec3(0.0f, 0.0f, 1.0f), pos, 50.0f, 6.0f);
	Enemy.SetLightPosition(0.0f, 1.0f, 0.0f);
	Enemy.SetTexture(SHADER_SAMPLER2D_MAIN_TEXTURE,
		"res/objs/hunter/textures/hunter_rearside.bmp");
	Enemy.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
	Enemy.SetCallback(EnemyProc);

	sound.Init();
	sound.LoadAudio("res/sound/thud.ogg", &thud);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
}

void
MainScene::SetViewport(FLOAT width, FLOAT height)
{
	Scene::SetViewport(width, height);

	gWidth = width, gHeight = height;
	glViewport(0, 0, (INT)width, (INT)height);

	cam.Switch3D(45.0f, width / height);
	shadowCam.Switch2D(100.0f, 200.0f);

	cam.Update();
	cam.Rotate(180.0f, 0.0f, 0.0f);

	tb = TextBitmap::GetInstance();
	tb->Init("res/shader/v.vs", "res/shader/f.fs", width, height,
		"res/font/msyh.ttc");

	//SHADOW INIT
	shadow.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs",
		&shadowCam, (INT)gWidth, (INT)gHeight);
	shadow.Add(&terrain);
	shadow.Add(&raven);
	shadow.Add(&Bullet);
	shadow.Add(&Enemy);

	fbo.Init();
	fbo.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, gWidth, gHeight);
	fbo.AttachDepthBuffer("depth", gWidth, gHeight);
	fbo.AttachFinish();

	fsqProg.SetTexture(SHADER_SAMPLER2D_MAIN_TEXTURE, shadow.GetShadowMap());

}

void
MainScene::OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y)
{
	Scene::OnTouch(event, tindex, x, y);

	//	LOG_D("move x:%f, y:%f", x, y);

	switch (event)
	{
	case EVENT_DOWN:
	{
		FLOAT half_width = gWidth * 0.5f;
		FLOAT half_height = gHeight * 0.5f;

		x /= half_width;
		y /= half_height;

		cm::vec3 vec = cm::unproject(x, y, cam.GetViewMatrix(), cam.GetProjectionMatrix());

		FLOAT len = cm::rayPicking(vec, cam.GetEyePos(), raven.GetPosition());

		LOG_D("vec : %s, len : %f", vec.dump(), len);
		break;
	}
	case EVENT_MOVE:
	{
		cam.Rotate(x, y, 0.0f);
		break;
	}
	case EVENT_UP:

		FLOAT half_width = gWidth * 0.5f;
		FLOAT half_height = gHeight * 0.5f;

		x /= half_width;
		y /= half_height;

		cm::vec3 vec = cm::unproject(x, y, cam.GetViewMatrix(), cam.GetProjectionMatrix());
		Bullet.SetEmitDirection(vec.x * 0.5f, vec.y * 0.1f, abs(vec.z));
		Bullet.Emit();

		sound.SetPosition(PL_SOURCES, x*3.f, y*3.f, 3.f, thud);
		sound.Play(thud);
		break;
	}
}

void
MainScene::OnKey(UINT event, UCHAR chr)
{
	Scene::OnKey(event, chr);

	if (!isEnemy)
		isEnemy = TRUE;

	switch (event)
	{
	case EVENT_DOWN:
		switch (chr)
		{
		case 'W':
			bMoveForward = TRUE;
			break;
		case 'S':
			bMoveBackward = TRUE;
			break;
		case 'A':
			bMoveLeft = TRUE;
			break;
		case 'D':
			bMoveRight = TRUE;
			break;
		case 'K':
			bMoveUp = TRUE;
			break;
		case 'J':
			bMoveDown = TRUE;
			break;
		}
		break;
	case EVENT_MOVE:
		break;
	case EVENT_UP:

		if (isGameover)
		{
			Stop();
			return;
		}

		switch (chr)
		{
		case 'W':
			bMoveForward = FALSE;
			break;
		case 'S':
			bMoveBackward = FALSE;
			break;
		case 'A':
			bMoveLeft = FALSE;
			break;
		case 'D':
			bMoveRight = FALSE;
			break;
		case 'K':
		{
			bMoveUp = FALSE; /*cam.PopMatrix();*/
			UCHAR * buf = SceneManager::GetInstance()->CaptureScene();

			LOG_D("%d, %d, %d", buf[0], buf[1], buf[2]);
			fsq.SetTexture(buf, gWidth, gHeight);
			fsq.Move(-0.1f, 0.0f, RIGHT_TOP_SCREEN);

			delete buf;
			break;
		}
		case 'J':
			bMoveDown = FALSE; /*cam.PushMatrix();*/
			break;
		case 'M':
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case 'N':
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 'V':
			bShowDepth = !bShowDepth;
			break;
		case 'F':
			Bullet.Emit();
			break;
		case 'Z':
			Enemy.Emit((FLOAT)(rand() % 100 - 50), 0.0f, 0.0f);
			break;
		case 'B':
			Stop();
			break;
		}
		break;

	}

}

void
MainScene::EventUpdate(FLOAT s)
{
	FLOAT z = 0.0f;
	FLOAT x = 0.0f;
	if (bMoveForward)
		z = -(s * MOVE_RATIO);
	//if (bMoveBackward)
	//	z = (s * MOVE_RATIO);
	if (bMoveLeft)
		x = -(s * MOVE_RATIO);
	if (bMoveRight)
		x = (s * MOVE_RATIO);

	if (x || z)
		Move(x, 0.0f, z);
}

void
MainScene::Move(FLOAT x, FLOAT y, FLOAT z)
{
	LOG_D("MOVE x : %f, y : %f, z : %f", x, y, z);
	//cam.Move(x, y, z);
	raven.Move(x, y, z);
	cm::vec3 pos = raven.GetPosition();

	Enemy.SetPosition(pos.x, pos.y, pos.z + 200.0f);
	Enemy.Rotate(180.0f, 0.0f, 1.0f, 0.0f);
	Bullet.SetPosition(pos);
	pos.y += 50.0f;
	pos.z += 80.0f;
	shadowCam.SetPosition(pos);
}

void
MainScene::Draw(FLOAT s)
{
	Scene::Draw(s);

	EventUpdate(s);
	Bullet.Update(s);
	Enemy.Update(s);

	fall_anim.Update(s);

	cam.Update();
	shadowCam.Update();
	shadow.Draw();

	CLEAR_COLOR(0.0f, 0.0f, 0.0f);

	DEPTH_TEST_END;
	skybox.Draw();
	DEPTH_TEST_BEGIN;

	if (bShowDepth)
	{
		fsqProg.Bind();
		fsq.Draw();
		fsqProg.Unbind();
	}

	terrain.Draw();

	raven.Draw();

	Bullet.Draw();
	Enemy.Draw();

	// TEST

	// Calculate Frames Per Second
	BLEND_BEGIN;
	currentMS += s;
	framesPerSecond++;
	if (currentMS >= 1.0f)
	{
		FPS = framesPerSecond;
		framesPerSecond = 0;
		currentMS = 0;

		// AUTO EMIT
		//Bullet.Emit();
		if(isEnemy)
			Enemy.Emit((FLOAT)(rand() % 100 - 50), 0.0f, 0.0f);

		if (score > 10)
			score -= 10;
		else
			score = 0;
	}
	swprintf(str, 120, L"FramesPerSecond : %d\0", FPS);
	tb->SetAlpha(1.0f);
	tb->Draw(str, -gWidth * 0.5f, gHeight * 0.5f - 30);


	swprintf(str, 120, L"Score : %d\0", score);
	tb->SetAlpha(1.0f);
	tb->Draw(str, 0.0f, gHeight * 0.5f - 30);

	if (textTime < 1.0f)
	{
		textTime += (s);
		swprintf(str, 120, L"Лїжа!!\0");
		tb->SetAlpha(POW_2(1.0f - textTime));
		tb->Draw(str, gWidth * 0.5f * textPos.x, gHeight * 0.5f * textPos.y);
	}

	if (isGameover)
	{

		swprintf(str, 120, L"Game Over, Press any key continue.\0");
		tb->Draw(str, -100.0f, 0.0f);
	}
	BLEND_END;
}

void MainScene::Start()
{
	terrain.PopMatrix();
	raven.PopMatrix();
	cam.PopMatrix();

	cam.PushMatrix();
	terrain.PushMatrix();
	raven.PushMatrix();

	Enemy.Clear();
	Bullet.Clear();

	bMoveForward = FALSE, bMoveBackward = FALSE, bMoveLeft = FALSE, bMoveRight =
		FALSE, bMoveUp = FALSE, bMoveDown = FALSE;

	raven.ResetPosition();
	raven.Move(0.0f, 20.0f, 0.0f);
}

void MainScene::Stop()
{
	Enemy.Clear();
	Bullet.Clear();
	bMoveForward = FALSE;
	bMoveBackward = FALSE;
	bMoveLeft = FALSE;
	bMoveRight = FALSE;
	bMoveUp = FALSE;
	bMoveDown = FALSE;
	isGameover = FALSE;
	isEnemy = FALSE;

	SceneManager::GetInstance()->Next("WELC");
}

void
WelcScene::Init()
{
	mCam.Init();
}

void
WelcScene::SetViewport(FLOAT width, FLOAT height)
{
	mCam.Switch3D(40.0f, width / height);

	bm.Init("res/shader/imagesprite.vs", "res/shader/imagesprite.fs", width,
		height, "res/font/msyh.ttc");
	bm.New("res/images/earth.bmp", L"START", 0.0f, 0.0f, 100.0f, 50.0f, []()->void
	{
		LOG_D("START BTN Clicked");
		SceneManager::GetInstance()->Next("MAIN");
		((MainScene*)SceneManager::GetInstance()->GetScene("MAIN"))->Start();
	});
}

void
WelcScene::OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y)
{
	switch (event)
	{
	case EVENT_DOWN:
		bm.OnTouchDown(x, y);
		break;
	case EVENT_MOVE:
	{
		mCam.Rotate(x, y, 0.0f);
	}
	break;
	case EVENT_UP:
		bm.OnTouchUp(x, y);
		break;
	}
}

void
WelcScene::OnKey(UINT event, UCHAR chr)
{
	switch (event)
	{
	case EVENT_DOWN:
		break;
	case EVENT_MOVE:
		break;
	case EVENT_UP:
		switch (chr)
		{
		case 'B':
			SceneManager::GetInstance()->Next("MAIN");
			((MainScene*)SceneManager::GetInstance()->GetScene("MAIN"))->Start();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void
WelcScene::Draw(FLOAT s)
{
	CLEAR_COLOR(0.1f, 0.2f, 0.4f);
	mCam.Update();

	bm.Draw();
}

void OverScene::Init()
{

	mCam.Init();
}

void OverScene::SetViewport(FLOAT width, FLOAT height)
{
	mCam.Switch3D(40.0f, width / height);

	bm.Init("res/shader/imagesprite.vs", "res/shader/imagesprite.fs", width,
		height, "res/font/msyh.ttc");
	bm.New("res/images/earth.bmp", L"START", 0.0f, 0.0f, 100.0f, 50.0f, []()->void
	{
		LOG_D("START BTN Clicked");
	});
}

void OverScene::OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y)
{
}

void OverScene::OnKey(UINT event, UCHAR chr)
{
}

void OverScene::Draw(FLOAT s)
{
	CLEAR_COLOR(0.0f, 0.0f, 0.0f);
	bm.Draw();
}
