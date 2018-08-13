#include "define.h"

#include "demo.h"

#include "opengltest.h"


SceneManager * manager;

void Init()
{
	manager = SceneManager::GetInstance();
	manager->AddScene("MAIN", new MainScene);
	manager->AddScene("WELC", new WelcScene);
	manager->InitScenes();
	manager->Next("MAIN");
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