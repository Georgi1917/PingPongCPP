#include <iostream>
#include "Window.h"
using namespace std;

const int timerID = 1;
const int timerInterval = 100;

HWND leftRect;
int leftX{ 50 }, leftY{ 50 };
const int leftWidth{ 8 }, leftHeight{ 40 };

HWND rightRect;
int rightX{ 560 }, rightY{ 370 };
const int rightWidth{ 8 }, rightHeight{ 40 };

HWND ballObj;
int ballX{ 320 }, ballY{ 240 }, ballDX{ -4 }, ballDY{ -4 };
const int ballWidth{ 8 }, ballHeight{ 8 };

void CheckLeftCollision(HWND rectObj, HWND ball) {

	RECT rect1, rect2, intersection;

	GetWindowRect(rectObj, &rect1);
	GetWindowRect(ball, &rect2);

	if (IntersectRect(&intersection, &rect1, &rect2)) {

		int midY = ((rect1.top + rect1.bottom) / 2);

		if (rect2.bottom < midY) {
			ballDX = 5;
			ballDY = -5;
		}
		else if (rect2.bottom >= midY) {
			ballDX = 5;
			ballDY = 5;
		}
		else if (rect2.bottom == midY) {
			ballDX = 5;
			ballDY = 0;
		}

	}

}

void CheckRightCollision(HWND rectObj, HWND ball) {

	RECT rect1, rect2, intersection;

	GetWindowRect(rectObj, &rect1);
	GetWindowRect(ball, &rect2);

	if (IntersectRect(&intersection, &rect1, &rect2)) {

		int midY = (rect1.top + rect1.bottom) / 2;

		if (rect2.bottom < midY) {
			ballDX = -5;
			ballDY = -5;
		}
		else if (rect2.bottom > midY) {
			ballDX = -5;
			ballDY = 5;
		}
		else if (rect2.bottom == midY) {
			ballDX = -5;
			ballDY = 0;
		}

	}

}

bool CheckLeftLoss(HWND leftRect, HWND ballObj) {
	RECT rect1, rect2;

	GetWindowRect(leftRect, &rect1);
	GetWindowRect(ballObj, &rect2);

	if (rect1.left > rect2.right) return true;

	return false;
}

bool CheckRightLoss(HWND rightRect, HWND ballObj) {
	RECT rect1, rect2;

	GetWindowRect(rightRect, &rect1);
	GetWindowRect(ballObj, &rect2);

	if (rect1.right < rect2.left) return true;

	return false;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) 
	{
	case WM_CREATE: {
		leftRect = CreateWindow(
			L"STATIC",
			NULL,
			WS_CHILD | WS_VISIBLE | SS_WHITERECT,
			leftX, leftY, leftWidth, leftHeight,
			hWnd,
			NULL,
			NULL,
			NULL
		);

		rightRect = CreateWindow(
			L"STATIC",
			NULL,
			WS_CHILD | WS_VISIBLE | SS_WHITERECT,
			rightX, rightY, rightWidth, rightHeight,
			hWnd,
			NULL,
			NULL,
			NULL
		);

		ballObj = CreateWindow(
			L"STATIC",
			NULL,
			WS_CHILD | WS_VISIBLE | SS_WHITERECT,
			ballX, ballY,
			ballWidth, ballHeight,
			hWnd,
			NULL,
			NULL,
			NULL
		);

		SetTimer(hWnd, timerID, timerInterval, NULL);

		break;
	}

	case WM_TIMER:
		ballX += ballDX;
		ballY += ballDY;

		cout << ballX << ", " << ballY << endl;

		RECT ballClientRect;
		GetClientRect(hWnd, &ballClientRect);

		// Checks for out of bounds
		if (ballX <= 0 || ballX + ballWidth >= ballClientRect.right) ballDX = -ballDX;
		if (ballY <= 0 || ballY + ballHeight >= ballClientRect.bottom) ballDY = -ballDY;

		// Checks for collision with left or right RECT
		CheckLeftCollision(leftRect, ballObj);
		CheckRightCollision(rightRect, ballObj);

		if (CheckLeftLoss(leftRect, ballObj) || CheckRightLoss(rightRect, ballObj)) cout << "You Lost!" << endl;

		SetWindowPos(ballObj, NULL, ballX, ballY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

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