#include <iostream>
#include <Windows.h>
#include <d3d9.h>
#include "FrameBuffer.h"
using namespace std;

FrameBuffer fb;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	//Register window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),CS_CLASSDC,MsgProc,0,0,
					  GetModuleHandle(NULL),NULL,NULL,NULL,NULL,
					  L"DirectX Framework Window",NULL };
	wc.hIconSm = (HICON)LoadImage(hInst, MAKEINTRESOURCE(NULL), IMAGE_ICON, 16, 16, 0);
	wc.hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(NULL), IMAGE_ICON, 32, 32, 0);
	RegisterClassEx(&wc);

	//Create window
	RECT wr;
	wr.left = 100;
	wr.right = 800 + wr.left;
	wr.top = 100;
	wr.bottom = 600 + wr.top;
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hWnd = CreateWindowW(L"DirectX Framework Window", L"DirectX Framework",
		WS_OVERLAPPEDWINDOW, wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
		NULL, NULL, wc.hInstance, NULL);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// Initialize FrameBuffer
	fb.hWnd = hWnd;
	fb.Init();

	//Process messages
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			fb.ClearBackBuffer();
			fb.LockBackBuffer();
			for (int y = 0; y < 100; y++) {
				for (int x = 0; x < 100; x++) {
					((D3DCOLOR*)fb.rect.pBits)[x + (fb.rect.Pitch >> 2) * y] = D3DCOLOR_XRGB(255, 255, 255);
				}
			}
			fb.UnlockBackBuffer();
			fb.Back2FrontBuffer();
		}
	}

	//When WM_DESTROY,release all the variable
	UnregisterClass(L"DirectX Framework Window", wc.hInstance);
	fb.Destroy();

	return 0;
}