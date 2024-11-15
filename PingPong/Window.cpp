#include <iostream>
#include "Window.h"
using namespace std;

HWND leftRect;
int leftX{ 50 };
int leftY{ 50 };
const int leftWidth{ 20 };
const int leftHeight{ 60 };

HWND rightRect;
int rightX{ 560 };
int rightY{ 370 };
const int rightWidth{ 20 };
const int rightHeight{ 60 };

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) 
	{
	case WM_CREATE: {
		leftRect = CreateWindow(
			L"STATIC",
			NULL,
			WS_CHILD | WS_VISIBLE | SS_WHITERECT,
			50, 50, 20, 60,
			hWnd,
			NULL,
			NULL,
			NULL
		);

		rightRect = CreateWindow(
			L"STATIC",
			NULL,
			WS_CHILD | WS_VISIBLE | SS_WHITERECT,
			560, 370, 20, 60,
			hWnd,
			NULL,
			NULL,
			NULL
		);

		break;
	}

	case WM_KEYDOWN:

		switch (wParam)
		{
		case 'W':
			leftY -= 10;
			break;
		case VK_UP:
			rightY -= 10;
			break;
		case 'S':
			leftY += 10;
			break;
		case VK_DOWN:
			rightY += 10;
			break;
		}
	

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		if (leftY < 0) leftY = 0;
		if (rightY < 0) rightY = 0;
		if (leftY + leftHeight > clientRect.bottom) leftY = clientRect.bottom - leftHeight;
		if (rightY + rightHeight > clientRect.bottom) rightY = clientRect.bottom - rightHeight;

		SetWindowPos(leftRect, NULL, leftX, leftY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		SetWindowPos(rightRect, NULL, rightX, rightY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window()
	: hInstance(GetModuleHandle(nullptr))
{
	const wchar_t* CLASS_NAME = L"Sample Window";

	WNDCLASS windowClass = {};

	windowClass.lpszClassName = CLASS_NAME;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));

	RegisterClass(&windowClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	int width{ 640 }, height{ 480 };

	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false);

	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Ping Pong",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, SW_SHOW);
}

Window::~Window()
{
	const wchar_t* CLASS_NAME = L"Sample Window";

	UnregisterClass(CLASS_NAME, hInstance);
}

bool Window::ProccessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}