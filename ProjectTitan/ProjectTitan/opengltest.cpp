#include "define.h"

#include "esm.hpp"
#include "opengltest.h"

#include "buttons.h"
#include "utils.h"
#include "program.h"
#include "model.h"
#include "fullscreenquad.h"
#include "framebuffer.h"
#include "animation.h"
#include "list.hpp"
#include "terrain.h"
#include "TextBitmap.h"

FLOAT gWidth, gHeight;

esm::mat4 viewMat, projectMat;

Camera cam, shadow;

Program prog, waterProg, modelProg, shadowProg;
Model Sphere, Cube;
Terrain terrain, water;

Animation anim;
Animation scaleAnim;

FrameBuffer shadowFbo;
FullScreenQuad fsq;
Program fsqProg;

TextBitmap tb;

void Init()
{
	LOG_D("%s", (const CHAR*)glGetString(GL_VERSION));

	cam.Init(esm::vec3(10.0f, 10.0f, 0.0f), esm::vec3(10.0f, 10.0f, -1.0f), esm::vec3(0.0f, 1.0f, 0.0f));
	shadow.Init(esm::vec3(0.0f, 30.0f, 0.0f), esm::vec3(0.0f, 0.0f, 0.0f), esm::vec3(0.0f, 0.0f, -1.0f));

	fsq.Init();
	fsqProg.Init("res/shader/fullscreenquad.vs", "res/shader/fullscreenquad.fs", &cam);

	prog.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &cam);
	prog.SetVector4f("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);
	prog.SetVector4f("U_DiffuseMaterial", 0.5f, 0.5f, 0.5f, 1.0f);
	//prog.SetVector4f("U_SpecularMaterial", 0.8f, 0.8f, 0.8f, 1.0f);
	prog.SetVector4f("U_LightPos", 0.0f, 10.0f, 0.0f, 0.0f);
	prog.SetTexture("U_Texture", "res/images/grass.bmp");

	waterProg.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &cam);
	waterProg.SetVector4f("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);
	waterProg.SetVector4f("U_DiffuseMaterial", 0.5f, 0.5f, 0.5f, 1.0f);
	waterProg.SetVector4f("U_SpecularMaterial", 1.0f, 1.0f, 1.0f, 1.0f);
	waterProg.SetVector4f("U_LightPos", 0.0f, 10.0f, 0.0f, 0.0f);
	waterProg.SetVector4f("U_WaterOption", 0.5f, 2.0f, 5.0f, 0.0f);
	waterProg.SetTexture("U_Texture", "res/images/waterHeight.bmp");

	modelProg.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &cam);
	modelProg.SetVector4f("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);
	modelProg.SetVector4f("U_DiffuseMaterial", 0.5f, 0.5f, 0.5f, 1.0f);
	modelProg.SetVector4f("U_SpecularMaterial", 1.0f, 1.0f, 1.0f, 1.0f);
	modelProg.SetVector4f("U_LightPos", 0.0f, 10.0f, 0.0f, 0.0f);
	modelProg.SetTexture("U_Texture", (UINT)0);

	shadowProg.Init("res/shader/blin_shadow.vs", "res/shader/blin_shadow.fs", &shadow);
	shadowProg.SetVector4f("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);
	shadowProg.SetVector4f("U_DiffuseMaterial", 0.5f, 0.5f, 0.5f, 1.0f);
	shadowProg.SetVector4f("U_SpecularMaterial", 1.0f, 1.0f, 1.0f, 1.0f);
	shadowProg.SetVector4f("U_LightPos", 0.0f, 10.0f, 0.0f, 0.0f);
	shadowProg.SetTexture("U_Texture", (UINT)0);

	shadowFbo.Init();

	Sphere.Init(prog.GetProgramId(), "res/objs/Sphere.obj");
	Sphere.Move(20.0f, 15.0f, 30.0f);
	Sphere.SetSize(5.0f, 5.0f, 5.0f);

	Cube.Init(prog.GetProgramId(), "res/objs/monkey.obj");
	Cube.Move(20.0f, 20.0f, 10.0f);

	terrain.Init(prog.GetProgramId(), "res/images/height_256x256.bmp");
	//terrain.Init(prog.GetProgramId(), "res/images/waterHeight.bmp");
	terrain.SetSize(1.0f, 20.0f, 1.0f);
	//terrain.SetPosition(-25.0f, 0.0f, -25.0f);

	water.Init(prog.GetProgramId(), nullptr);
	water.SetSize(1.0f, 1.0f, 1.0f);
	//water.SetPosition(-25.0f, 0.0f, -25.0f);

	//glm::angleAxis()
	//glm::mat4_cast()

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
}

void SetViewportSize(FLOAT width, FLOAT height)
{
	gWidth = width, gHeight = height;
	glViewport(0, 0, width, height);

	cam.Switch3D(45.0f, width / height);
	//cam.Switch2D(gWidth, gHeight);

	shadow.Switch2D(200.0f, 200.0f);

	shadowFbo.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, gWidth, gHeight);
	shadowFbo.AttachDepthBuffer("depth", gWidth, gHeight);
	shadowFbo.AttachFinish();

	prog.SetTexture("U_ShadowMap", shadowFbo.GetBuffer("depth"));
	waterProg.SetTexture("U_ShadowMap", shadowFbo.GetBuffer("depth"));
	modelProg.SetTexture("U_ShadowMap", shadowFbo.GetBuffer("depth"));
	fsqProg.SetTexture("U_Texture", shadowFbo.GetBuffer("depth"));

	Cube.SetShadowCamera(shadow);
	Sphere.SetShadowCamera(shadow);
	terrain.SetShadowCamera(shadow);
	water.SetShadowCamera(shadow);

	tb.Init("res/font/msyh.ttc",width,height, 32, 0, 0, 1);
}

esm::vec3 positions[] =
{
	{-3.0f, 3.0f, 0.0f}, {-1.5f, 3.0f, 0.0f}, {0.0f, 3.0f, 0.0f}, {1.5f, 3.0f, 0.0f}, {3.0f, 3.0f, 0.0f},
	{-3.0f, 1.5f, 0.0f}, {-1.5f, 1.5f, 0.0f}, {0.0f, 1.5f, 0.0f}, {1.5f, 1.5f, 0.0f}, {3.0f, 1.5f, 0.0f},
	{-3.0f, 0.0f, 0.0f}, {-1.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.5f, 0.0f, 0.0f}, {3.0f, 0.0f, 0.0f},
	{-3.0f, -1.5f, 0.0f}, {-1.5f, -1.5f, 0.0f}, {0.0f, -1.5f, 0.0f}, {1.5f, -1.5f, 0.0f}, {3.0f, -1.5f, 0.0f},
	{-3.0f, -3.0f, 0.0f}, {-1.5f, -3.0f, 0.0f}, {0.0f, -3.0f, 0.0f}, {1.5f, -3.0f, 0.0f}, {3.0f, -3.0f, 0.0f}
};

FLOAT waterOffset = 0.0f;
BOOL bShowDepth = FALSE;

static UINT currentMS = 0;
UINT FPS = 0;
UINT framesPerSecond = 0;

void Draw(UINT elapse)
{

	shadowFbo.Bind();
	shadowProg.Bind();
	Sphere.Draw();
	terrain.Draw();
	water.Draw();
	Cube.Draw();
	shadowProg.Unbind();
	shadowFbo.Unbind();

	glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (bShowDepth)
	{
		fsqProg.Bind();
		fsq.Draw();
	}

	prog.SetVector4f("U_CameraPos", cam.mEye.x, cam.mEye.y, cam.mEye.z, 0.0f);
	prog.Bind();
	terrain.Draw();
	prog.Unbind();

	waterProg.Bind();
	waterProg.SetVector4f("U_CameraPos", cam.mEye.x, cam.mEye.y, cam.mEye.z, 0.0f);
	waterProg.SetVector4f("U_WaterOption", 0.5f, waterOffset += 0.05f, 2.0f, 0.0f);
	water.Draw();
	waterProg.Unbind();

	modelProg.Bind();
	modelProg.SetVector4f("U_CameraPos", cam.mEye.x, cam.mEye.y, cam.mEye.z, 0.0f);
	Cube.Draw();
	Sphere.Draw();
	modelProg.Unbind();


	// Calculate Frames Per Second
	currentMS += elapse;
	framesPerSecond++;
	if (currentMS >= 1000)
	{
		FPS = framesPerSecond;
		framesPerSecond = 0;
		currentMS = 0;
	}
	WCHAR * str = new WCHAR[128];
	swprintf(str, L"FPS:%d\0", FPS);
	tb.Draw(-gWidth * 0.5f, gHeight * 0.5f - 30, str);
}

void OnTouch(FLOAT x, FLOAT y)
{
	x = x / gWidth * 2.0f - 1.0f;
	y = y / gHeight * 2.0f - 1.0f;
	y *= -1.0f;

	esm::vec3 pos = esm::unProject(x, y, cam.GetViewMatrix(), cam.GetProjectionMatrix());
	std::cout << pos.dump() << std::endl;

	//NEW TEST : y = ax + b
	FLOAT a1 = pos.y / pos.z;
	FLOAT newY = a1 * (-10.0f - cam.mEye.z);

	FLOAT a2 = pos.x / pos.z;
	FLOAT newX = a2 * (-10.0f + cam.mEye.z);
	std::cout << newX << ", " << newY << std::endl;

	bShowDepth = !bShowDepth;
}

void Move(FLOAT x, FLOAT y, FLOAT z)
{
	cam.Move(x, y, z);
}

void RotateViewDir(FLOAT yaw, FLOAT pitch, FLOAT roll)
{
	cam.Rotate(yaw, pitch, roll);
}

void CameraForward(float offset)
{
}
