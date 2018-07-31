#include "define.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <Windows.h>

#include "opengltest.h"
#include "utils.h"

char szClassName[] = "OpenGLWin";
char szWinName[] = "OpenGLWindows";

bool bMoveForward, bMoveBackward, bMoveLeft, bMoveRight, bMoveUp, bMoveDown;
::POINT originPos;
bool bCaptureCursor;


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
		WGL_DEPTH_BITS_ARB, 24,
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

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
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
	{ 0, 0, 1024, 762 };
	AdjustWindowRect(&screen, WS_OVERLAPPEDWINDOW, 0);

	float screenWidth = screen.right - screen.left;
	float screenHeight = screen.bottom - screen.top;

	HWND hWnd =
		CreateWindowEx(0, szClassName, szWinName, WS_OVERLAPPEDWINDOW, 100, 100, screenWidth, screenHeight, 0, 0, hInstance, 0);

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
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormatID = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelFormatID, &pfd);

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	glewInit();

	if (wglCreateContextAttribsARB)
	{
		wglDeleteContext(rc);
		ReleaseDC(hWnd, dc);
		DestroyWindow(hWnd);

		hWnd = CreateWindowEx(0, szClassName, szWinName, WS_OVERLAPPEDWINDOW, 100, 100, screenWidth, screenHeight, 0, 0, hInstance, 0);
		rc = CreateGLRC(hWnd);
		dc = GetDC(hWnd);
		wglMakeCurrent(dc, rc);
	}

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

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

		static UINT hTime = 0;
		UINT currentTimeElapse = GetTimeElapse(&hTime);

		Draw(currentTimeElapse);

		if (bMoveForward)
			Move(0.0f, 0.0f, -(currentTimeElapse*MOVE_RATIO));
		if (bMoveBackward)
			Move(0.0f, 0.0f, (currentTimeElapse*MOVE_RATIO));
		if (bMoveLeft)
			Move(-(currentTimeElapse*MOVE_RATIO), 0.0f, 0.0f);
		if (bMoveRight)
			Move((currentTimeElapse*MOVE_RATIO), 0.0f, 0.0f);

		if (bMoveUp)
			Move(0.0f, (currentTimeElapse*MOVE_RATIO), 0.0f);
		if (bMoveDown)
			Move(0.0f, -(currentTimeElapse*MOVE_RATIO), 0.0f);

		SwapBuffers(dc);

	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static ::POINT cursorPos;
	memset(&cursorPos, 0, sizeof(::POINT));
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		originPos.x = lParam & 0xFFFF;
		originPos.y = lParam >> 16 & 0xFFFF;
		ClientToScreen(hWnd, &originPos);
		bCaptureCursor = true;
		SetCursor(LoadCursor(0, IDC_HAND));
		std::cout << "Cursor position : x " << (lParam & 0xFFFF) << ", y " << (lParam >> 16 & 0xFFFF) << std::endl;
		break;
	case WM_RBUTTONUP:
		{
			FLOAT x = LOWORD(lParam);
			FLOAT y = HIWORD(lParam);
			OnTouch(x, y);
		}
		break;
	case WM_MOUSEMOVE:
		if (bCaptureCursor)
		{
			SetCursor(LoadCursor(0, IDC_HAND));
			cursorPos = { LOWORD(lParam), HIWORD(lParam) };
			ClientToScreen(hWnd, &cursorPos);
			float x = (cursorPos.x - originPos.x) / 800.0f;
			float y = -(cursorPos.y - originPos.y) / 600.0f;
			//LOG_D("%f, %f", x, y);
			RotateViewDir(x, y);
			SetCursorPos(originPos.x, originPos.y);
		}
		break;
	case WM_LBUTTONUP:
		bCaptureCursor = false;
		SetCursorPos(originPos.x, originPos.y);
		SetCursor(LoadCursor(0, IDC_ARROW));
		//ShowCursor(true);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'Q':PostQuitMessage(0); break;
		case 'W':bMoveForward = true; break;
		case 'S':bMoveBackward = true; break;
		case 'A':bMoveLeft = true; break;
		case 'D':bMoveRight = true; break;
		case 'K':bMoveUp = true; break; 
		case 'J':bMoveDown = true; break;
		}
		//std::cout << (char)wParam << std::endl;
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'Q': PostQuitMessage(0); break;
		case 'W': bMoveForward = false; break;
		case 'S': bMoveBackward = false; break;
		case 'A': bMoveLeft = false; break;
		case 'D': bMoveRight = false; break;
		case 'K': bMoveUp = false; break;
		case 'J': bMoveDown = false; break;
		case 'O': CameraForward(-1.0f); break;
		case 'P': CameraForward(1.0f); break;
		case 'M': glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
		case 'N': glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
		}
		std::cout << "Keyword : " << (char)wParam << std::endl;
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
