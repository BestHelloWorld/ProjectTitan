#include "core/define.h"

#include "demo.h"

#include "opengltest.h"


SceneManager * manager;

void Init()
{
	MainScene * scene = new MainScene;
	manager = SceneManager::GetInstance();
	manager->AddScene("MAIN", scene);
	manager->AddScene("WELC", new WelcScene);
	manager->AddScene("OVER", new OverScene);
	manager->InitScenes();
	manager->Next("WELC");
}

void SetViewportSize(FLOAT width, FLOAT height)
{
	manager->SetViewport(width, height);
}

void Draw(FLOAT elapse)
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