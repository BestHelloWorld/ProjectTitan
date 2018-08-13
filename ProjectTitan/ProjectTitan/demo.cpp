#include "demo.h"



Unit Raven;

// MODEL EMIT
ModelEmit Bullet;
ModelEmit Enemy;


FLOAT textTime;
esm::vec4 textPos;

Terrain terrain;
Aircraft raven;

Program spriteProg;
ImageSprite imgSprite;
ButtonManager sm;

void __stdcall ModelProc(Model * model)
{
	if (model->mIsHide)
		return;
	for (INT i = 0; i < Enemy.mModels.size(); ++i)
	{
		//esm::vec3 pos1 = Raven.GetPosition();
		esm::vec3 pos1 = (*Enemy.mModels[i]).GetPosition();
		esm::vec3 pos2 = model->GetPosition();
		FLOAT distance = ((pos1 - pos2) * 0.5f).magnitude();
		if (distance <= 1.0f)
		{
			textPos = Bullet.mProgram->mCamera->WorldToScreen(pos1);
			textPos = textPos / textPos.w;
			CHAR * t = textPos.dump();
			LOG_D("BINGO : %s", t);
			model->Hide();
			Enemy.mModels[i]->SetMoveAdd(0.0f, -0.1f, 0.0f);
			textTime = 0.0f;
		}
	}
}

void MainScene::Init()
{
	Scene::Init();

	LOG_D("%s", (const CHAR*)glGetString(GL_VERSION));

	bMoveForward = FALSE, bMoveBackward = FALSE, bMoveLeft = FALSE, bMoveRight = FALSE, bMoveUp = FALSE, bMoveDown = FALSE;

	cam.Init(esm::vec3(0.0f, 5.0f, 0.0f), esm::vec3(0.0f, 5.0f, -1.0f), esm::vec3(0.0f, 1.0f, 0.0f));
	shadowCam.Init(esm::vec3(0.0f, 50.0f, 0.0f), esm::vec3(0.0f, 0.0f, 0.0f), esm::vec3(0.0f, 0.0f, -1.0f));
	cam.CameraForward(-30.0f);

	skybox.Init("res/shader/skybox.vs", "res/shader/skybox.fs", &cam, 
		"res/images/skybox/front.bmp",
		"res/images/skybox/back.bmp", 
		"res/images/skybox/left.bmp", 
		"res/images/skybox/right.bmp", 
		"res/images/skybox/top.bmp", 
		"res/images/skybox/bottom.bmp");
	skybox.SetSize(10.0f);

	//spriteProg.Init("res/shader/v.vs", "res/shader/f.fs", &cam);
	//imgSprite.Init(spriteProg.GetProgramId(), "res/images/earth.bmp", 0.0f, 0.0f, 100.0f, 100.0f);

	fsq.Init(RIGHT_TOP_SCREEN);
	fsqProg.Init("res/shader/fullscreenquad.vs", "res/shader/fullscreenquad.fs", &cam);

	//TERRAIN INIT
	terrain.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &cam, "res/images/height_256x256.bmp");
	terrain.SetTexture(SHADER_SAMPLER2D_MAIN_TEXTURE, "res/images/grass.bmp");
	terrain.SetLightPos(0.0f, 10.0f, 0.0f, 0.0f, FALSE);
	*terrain.mModelMatrix = *terrain.mModelMatrix * esm::rotate(45.0f, 0.0f, 1.0f, 0.0f);
	terrain.Move(-100.0f, 0.0f, -100.0f);
	terrain.SetSize(5.0f, 20.0f, 5.0f);

	//MODEL INIT
	raven.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", "res/objs/raven.obj", &cam);
	raven.SetLightPos(0.0f, 1.0f, 0.0f);
	raven.SetTexture("U_Texture", "res/images/raven.bmp");
	raven.BindCamera(&cam);
	raven.Move(0.0f, 20.0f, 0.0f);

	Bullet.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &cam, "res/objs/rocket.obj", esm::vec3(0.0f, 0.0f, 1.0f), raven.GetPosition(), 100.0f);
	//Bullet.Scale(0.2f, 0.2f, 0.2f);
	Bullet.SetLightPosition(0.0f, 1.0f, 0.0f);
	Bullet.SetCallback(ModelProc);

	Enemy.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &cam, "res/objs/hunter/sources/hunter_rearside.obj", esm::vec3(0.0f, 0.0f, -1.0f), raven.GetPosition(), 50.0f, 10.0f);
	Enemy.SetLightPosition(0.0f, 1.0f, 0.0f);
	Enemy.SetTexture(SHADER_SAMPLER2D_MAIN_TEXTURE, "res/objs/hunter/textures/hunter_rearside.bmp");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
}

void MainScene::SetViewport(FLOAT width, FLOAT height)
{
	Scene::SetViewport(width, height);

	gWidth = width, gHeight = height;
	glViewport(0, 0, width, height);

	cam.Switch3D(45.0f, width / height);
	shadowCam.Switch2D(200.0f, 200.0f);

	tb = TextBitmap::GetInstance();
	tb->Init("res/shader/v.vs", "res/shader/f.fs", width, height, "res/font/msyh.ttc", 32);

	//SHADOW INIT
	shadow.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &shadowCam, gWidth, gHeight);
	shadow.Add(&terrain);
	shadow.Add(&raven);
	shadow.Add(&Bullet);
	shadow.Add(&Enemy);

	fsqProg.SetTexture(SHADER_SAMPLER2D_MAIN_TEXTURE, shadow.GetShadowMap());
}

void MainScene::OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y)
{
	Scene::OnTouch(event, tindex, x, y);

	switch (event)
	{
	case EVENT_DOWN:
		break;
	case EVENT_MOVE:
	{
		cam.Rotate(x, y, 0.0f);
	}
	break;
	case EVENT_UP:
		break;
	}
}

void MainScene::OnKey(UINT event, UCHAR chr)
{
	Scene::OnKey(event, chr);

	switch (event)
	{
	case EVENT_DOWN:
		switch (chr)
		{
		case 'W':bMoveForward = TRUE; break;
		case 'S':bMoveBackward = TRUE; break;
		case 'A':bMoveLeft = TRUE; break;
		case 'D':bMoveRight = TRUE; break;
		case 'K':bMoveUp = TRUE; break;
		case 'J':bMoveDown = TRUE; break;
		}
		break;
	case EVENT_MOVE:
		break;
	case EVENT_UP:
		LOG_D("EVENT_UP : %c", chr);
		switch (chr)
		{
		case 'W': bMoveForward = FALSE; break;
		case 'S': bMoveBackward = FALSE; break;
		case 'A': bMoveLeft = FALSE; break;
		case 'D': bMoveRight = FALSE; break;
		case 'K': bMoveUp = FALSE; break;
		case 'J': bMoveDown = FALSE; break;
		case 'M': glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
		case 'N': glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
		case 'V': bShowDepth = !bShowDepth; break;
		case 'F': Bullet.Emit(); break;
		case 'Z': Enemy.Emit(); break;
		case 'B': SceneManager::GetInstance()->Next("WELC"); break;
		}
		break;

	}

}

void MainScene::EventUpdate(FLOAT s)
{
	FLOAT z = 0.0f;
	FLOAT x = 0.0f;
	if (bMoveForward)
		z = -(s*MOVE_RATIO);
	if (bMoveBackward)
		z = (s*MOVE_RATIO);
	if (bMoveLeft)
		x = -(s*MOVE_RATIO);
	if (bMoveRight)
		x = (s*MOVE_RATIO);

	if (x || z)
		Move(x, 0.0f, z);
}

void MainScene::Move(FLOAT x, FLOAT y, FLOAT z)
{
	raven.Move(x, y, z);
	esm::vec3 pos = raven.GetPosition();

	Enemy.SetPosition(pos.x, pos.y, pos.z + 200.0f);
	Bullet.SetPosition(pos);
	pos.y += 20.0f;
	shadowCam.SetPosition(pos);
}

void MainScene::Draw(FLOAT s)
{
	Scene::Draw(s);

	EventUpdate(s);
	Bullet.Update(s);

	cam.Update();
	shadowCam.Update();
	shadow.Draw();

	CLEAR_COLOR(0.1f, 0.2f, 0.4f);

	if (bShowDepth)
	{
		fsqProg.Bind();
		fsq.Draw();
		fsqProg.Unbind();
	}

	DEPTH_TEST_END;
	skybox.Draw();
	DEPTH_TEST_BEGIN;

	terrain.Draw();

	raven.Update(s);
	raven.Draw();

	//Bullet.Emit();
	Bullet.Update(s);
	Bullet.Draw();

	//Enemy.Emit();
	Enemy.Update(s);
	Enemy.Draw();

	// Calculate Frames Per Second
	BLEND_BEGIN;
	currentMS += s;
	framesPerSecond++;
	if (currentMS >= 1.0f)
	{
		FPS = framesPerSecond;
		framesPerSecond = 0;
		currentMS = 0;
	}
	//FPS = /*1.0f / */s;
	swprintf(str, 120, L"FramesPerSecond : %d\0", FPS);
	tb->SetAlpha(1.0f);
	tb->Draw(str, -gWidth * 0.5f, gHeight * 0.5f - 30);

	if (textTime < 1.0f)
	{
		textTime += (s);
		swprintf(str, 120, L"Лїжа!!! \0");
		tb->SetAlpha(POW_2(1.0f - textTime));
		tb->Draw(str, gWidth * 0.5f * textPos.x, gHeight * 0.5f * textPos.y);
	}
	BLEND_END;
}


void WelcScene::Init()
{
	mCam.Init();
}

void WelcScene::SetViewport(FLOAT width, FLOAT height)
{
	mCam.Switch3D(40.0f, width / height);
}

void WelcScene::OnTouch(UINT event, FLOAT tindex, FLOAT x, FLOAT y)
{
	switch (event)
	{
	case EVENT_DOWN:
		break;
	case EVENT_MOVE:
	{
		mCam.Rotate(x, y, 0.0f);
	}
	break;
	case EVENT_UP:
		break;
	}
}

void WelcScene::OnKey(UINT event, UCHAR chr)
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
		case 'N': SceneManager::GetInstance()->Next("MAIN"); break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void WelcScene::Draw(FLOAT s)
{
	CLEAR_COLOR(0.1f, 0.2f, 0.4f);
	mCam.Update();

}