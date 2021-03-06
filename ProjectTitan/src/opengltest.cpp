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
	manager->AddScene("TIME", new TimeScene);
	manager->InitScenes();

	manager->SetScene("WELC");
}

void SetViewportSize(FLOAT width, FLOAT height)
{
	manager->SetViewport(width, height);

	TextBitmap * tb = TextBitmap::GetInstance();
	tb->Init("res/shader/v.vs", "res/shader/f.fs", width, height,
		"res/font/msyh.ttc");
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
