#include "define.h"

#include "scene.h"
#include "demo.h"

#include "opengltest.h"

SceneManager * manager;

void Init()
{
	MainScene * scene = new MainScene;
	WelcScene * scene1 = new WelcScene;

	manager = SceneManager::GetInstance();
	manager->AddScene("MAIN", scene);
	manager->AddScene("WELC", scene1);
	manager->InitScenes();
}

void SetViewportSize(FLOAT width, FLOAT height)
{
	manager->SetViewport(width, height);
}

void Draw(UINT elapse)
{
	manager->Draw(MS_TO_S(elapse));
}

void OnTouch(UINT event, FLOAT x, FLOAT y)
{
	manager->OnTouch(event, 0, x, y);
}

void OnKey(UINT event, CHAR chr)
{
	manager->OnKey(event, chr);
}