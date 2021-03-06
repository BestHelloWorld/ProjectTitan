#define WIN_WIDTH	1024
#define WIN_HEIGHT	762

#include "core/define.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <Windows.h>

#include "opengltest.h"
#include "core/utils.h"

WCHAR szClassName[] = L"OpenGLWin";
WCHAR szWinName[] = L"OpenGLWindows";

POINT originPos;
POINT bufferPos;
bool bCaptureCursor;

INT screenWidth;
INT screenHeight;

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

HGLRC
CreateGLRC(HWND hWnd)
{
	HGLRC rc = 0;
	GLint attribs[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_RED_BITS_ARB, 8,
		WGL_GREEN_BITS_ARB, 8,
		WGL_BLUE_BITS_ARB, 8,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 8,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 16,
		0, 0
	};
	HDC dc = GetDC(hWnd);
	int pixelFormat[256] = { 0 };
	UINT format = 0;
	wglChoosePixelFormatARB(dc, attribs, 0, sizeof(pixelFormat) / sizeof(float), pixelFormat, &format);
	if (format > 0)
	{
		PIXELFORMATDESCRIPTOR pfd = { 0 };
		DescribePixelFormat(dc, pixelFormat[0], sizeof(pfd), &pfd);
		SetPixelFormat(dc, pixelFormat[0], &pfd);
		GLint contextAttrib[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0, 0
		};
		rc = wglCreateContextAttribsARB(dc, 0, contextAttrib);
	}
	return rc;
}

int main()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wce =
	{ 0 };
	wce.cbSize = sizeof(wce);
	wce.hCursor = LoadCursor(0, IDC_ARROW);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WinProc;
	wce.lpszClassName = szClassName;
	wce.style = CS_HREDRAW | CS_VREDRAW;

	ATOM atom = RegisterClassEx(&wce);

	::RECT screen =
	{ 0, 0, WIN_WIDTH, WIN_HEIGHT };
	AdjustWindowRect(&screen, WS_OVERLAPPEDWINDOW, 0);

	screenWidth = (screen.right - screen.left);
	screenHeight = (screen.bottom - screen.top);

	HWND hWnd =
		CreateWindowEx(0, szClassName, szWinName, WS_OVERLAPPEDWINDOW, 200, 200, screenWidth, screenHeight, 0, 0, hInstance, 0);

	if (!hWnd)
	{
		return -1;
	}

	GetClientRect(hWnd, &screen);
	int width = screen.right - screen.left;
	int height = screen.bottom - screen.top;

	HDC dc = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd =
	{ 0 };
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 8;
	pfd.cStencilBits = 8;

	int pixelFormatID = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelFormatID, &pfd);

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	glewInit();

	//if (wglCreateContextAttribsARB)
	//{
	//	wglDeleteContext(rc);
	//	ReleaseDC(hWnd, dc);
	//	DestroyWindow(hWnd);
	//	hWnd = CreateWindowEx(0, szClassName, szWinName, WS_OVERLAPPEDWINDOW, 200, 200, screenWidth, screenHeight, 0, 0, hInstance, 0);
	//	rc = CreateGLRC(hWnd);
	//	dc = GetDC(hWnd);
	//	wglMakeCurrent(dc, rc);
	//}

	ShowWindow(hWnd, SW_SHOW);

	// CALL OPENGL INIT FUNCTION
	Init();
	SetViewportSize(1024, 762);

	MSG msg =
	{ 0 };
	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		static FLOAT currentTimeElapse = 0.16f;
		static UINT hTime = 0;
		currentTimeElapse = GetTimeElapse(&hTime);

		// CALL OPENGL DRAW FUNCTION
		Draw(currentTimeElapse);

		SwapBuffers(dc);
	}

	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static POINT cursorPos;
	memset(&cursorPos, 0, sizeof(POINT));
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	{
		FLOAT x = static_cast<FLOAT>(LWORD(lParam));
		FLOAT y = static_cast<FLOAT>(HWORD(lParam));
		originPos.x = (LONG)x;
		originPos.y = (LONG)y;
		ClientToScreen(hWnd, (POINT*)&originPos);
		bCaptureCursor = true;

		FLOAT half_width = (FLOAT)WIN_WIDTH * 0.5f;
		FLOAT half_height = (FLOAT)WIN_HEIGHT * 0.5f;

		OnTouch(EVENT_DOWN, x - half_width, half_height - y);
	}
	break;
	case WM_MOUSEMOVE:
		if (bCaptureCursor)
		{
			SetCursor(LoadCursor(NULL, IDC_HAND));
			cursorPos = { LWORD(lParam), HWORD(lParam) };
			ClientToScreen(hWnd, &cursorPos);
			float x = (cursorPos.x - originPos.x) / 6.0f;
			float y = -(cursorPos.y - originPos.y) / 6.0f;
			OnTouch(EVENT_MOVE, x, y);
			SetCursorPos(originPos.x, originPos.y);
		}
		break;
	case WM_LBUTTONUP:
	{
		FLOAT x = static_cast<FLOAT>(LWORD(lParam));
		FLOAT y = static_cast<FLOAT>(HWORD(lParam));

		FLOAT half_width = (FLOAT)WIN_WIDTH * 0.5f;
		FLOAT half_height = (FLOAT)WIN_HEIGHT * 0.5f;

		OnTouch(EVENT_UP, x - half_width, half_height - y);

		bCaptureCursor = false;
		SetCursorPos(originPos.x, originPos.y);
	}
	break;
	case WM_KEYDOWN:
		OnKey(EVENT_DOWN, (CHAR)wParam);
		break;
	case WM_KEYUP:
		OnKey(EVENT_UP, (CHAR)wParam);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return NULL;
}
