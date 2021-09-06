#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#define SINGLE_STEP_PIXELS 3

#define DIRECTION int
#define LEFT 0
#define UP 4
#define RIGHT 8
#define DOWN 12

#define RECTANGLE 100
#define PAINT 200

const int IDM_Rectangle = 0;
const int IDM_Picture = 1;
const int IDM_Cut = 2;
const int rect_size = 50;
const int x_chor = 100;
const int y_chor = 100;
int status;

RECT movableRect = { x_chor, y_chor, x_chor + 50, y_chor + 50 };
RECT clientRect;

const HBRUSH MOVABLE_RECT_BRUSH = CreateSolidBrush(RGB(100, 149, 237));

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
void dragMovableRect(DIRECTION direction, int offset);
int getAllowedOffset(DIRECTION direction, int offset);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	/////////////////////////////////////////////////////
#pragma region 
	const wchar_t CLASS_NAME[] = L"Sample Window Class";
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,         // Size and position
		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}
#pragma endregion homeWindow
	ShowWindow(hwnd, nCmdShow);
	/// ////////////////////////////////////////////////////
#pragma region childWindow
	HWND child;
	WNDCLASSEX wchcl;
	const wchar_t CLASS_NAME2[] = L"Dialog box";
	wchcl.cbSize = sizeof(wchcl);
	wchcl.style = CS_HREDRAW | CS_HREDRAW;
	wchcl.lpfnWndProc = ChildProc;
	wchcl.cbClsExtra = 0;
	wchcl.cbWndExtra = 0;
	wchcl.hInstance = hInstance;
	wchcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wchcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wchcl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	wchcl.lpszMenuName = NULL;
	wchcl.lpszClassName = CLASS_NAME2;
	wchcl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wchcl))
		return 0;

	child = CreateWindowEx(
		0,
		CLASS_NAME2,
		CLASS_NAME2,
		WS_OVERLAPPEDWINDOW,
		0, 0, 150, 150,
		hwnd,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);
#pragma endregion childWindow
	/*ShowWindow(child, SW_SHOWNORMAL);*/

	///////////////////////////////////////////////////////
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hdc;
	switch (uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &paintStruct);
		GetClientRect(hwnd, &clientRect);
		FillRect(hdc, &clientRect, 0);
		FillRect(hdc, &movableRect, MOVABLE_RECT_BRUSH);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			dragMovableRect(LEFT, SINGLE_STEP_PIXELS);
			break;
		case VK_RIGHT:
			dragMovableRect(RIGHT, SINGLE_STEP_PIXELS);
			break;
		case VK_UP:
			dragMovableRect(UP, SINGLE_STEP_PIXELS);
			break;
		case VK_DOWN:
			dragMovableRect(DOWN, SINGLE_STEP_PIXELS);
			break;
		}
		InvalidateRect(hwnd, &clientRect, false);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ChildProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)                  /* handle the messages */
	{
	case WM_DESTROY:
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}

void dragMovableRect(DIRECTION direction, int offset) {

	offset = getAllowedOffset(direction, offset);

	switch (direction) {
	case UP:
		OffsetRect(&movableRect, 0, -offset);
		break;
	case DOWN:
		OffsetRect(&movableRect, 0, offset);
		break;
	case LEFT:
		OffsetRect(&movableRect, -offset, 0);
		break;
	case RIGHT:
		OffsetRect(&movableRect, offset, 0);
		break;
	}
}

int getAllowedOffset(DIRECTION direction, int offset) {
	LONG firstPosition;
	LONG secondPosition;

	switch (direction) {
	case UP:
		firstPosition = movableRect.top - offset;
		secondPosition = clientRect.top;
		break;
	case DOWN:
		firstPosition = clientRect.bottom;
		secondPosition = movableRect.bottom + offset;
		break;
	case LEFT:
		firstPosition = movableRect.left - offset;
		secondPosition = clientRect.left;
		break;
	case RIGHT:
		l

		firstPosition = clientRect.right;
		secondPosition = movableRect.right + offset;
		break;
	}

	if ((secondPosition - firstPosition) > -1) {
		return -10 * offset;
	}

	return offset;
}