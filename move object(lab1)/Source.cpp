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
const int IDM_Cut = 2;
const int rect_size = 50;
const int x_chor = 100;
const int y_chor = 100;
int status;

RECT movableRect = { x_chor, y_chor, x_chor + 50, y_chor + 50 };
RECT clientRect;

const HBRUSH MOVABLE_RECT_BRUSH = CreateSolidBrush(RGB(100, 149, 237));
const HBRUSH WINDOW_RECT_BRUSH = CreateSolidBrush(RGB(255, 245, 238));

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
void dragMovableRect(DIRECTION direction, int offset);
int getAllowedOffset(DIRECTION direction, int offset);
void correctChordsMouse();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
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
	#pragma endregion CreatingMainWindow
	ShowWindow(hwnd, nCmdShow);
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
	{
		hdc = BeginPaint(hwnd, &paintStruct);
		GetClientRect(hwnd, &clientRect);
		FillRect(hdc, &clientRect, WINDOW_RECT_BRUSH);
		FillRect(hdc, &movableRect, MOVABLE_RECT_BRUSH);
		EndPaint(hwnd, &paintStruct);
		break;
	}
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
	case WM_MOUSEMOVE: 
	{
		int mouseXPos = LOWORD(lParam) - (rect_size / 2);
		int mouseYPos = HIWORD(lParam) - (rect_size / 2);

		if ((mouseXPos > 0)
			&& (mouseXPos < clientRect.right - rect_size)
			&& (mouseYPos > 0)
			&& (mouseYPos < clientRect.bottom - rect_size)) {

			if (movableRect.top <= mouseYPos)  dragMovableRect(DOWN, mouseYPos - movableRect.top);
			if (movableRect.left >= mouseXPos) dragMovableRect(LEFT, movableRect.left - mouseXPos);
			if (movableRect.top >= mouseYPos)  dragMovableRect(UP, movableRect.top - mouseYPos);
			if (movableRect.left <= mouseXPos)	dragMovableRect(RIGHT, mouseXPos - movableRect.left);
		}

		correctChordsMouse();
		InvalidateRect(hwnd, &clientRect, false);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT ?
			GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? dragMovableRect(RIGHT, SINGLE_STEP_PIXELS) : dragMovableRect(LEFT, SINGLE_STEP_PIXELS) :
			GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? dragMovableRect(UP, SINGLE_STEP_PIXELS) : dragMovableRect(DOWN, SINGLE_STEP_PIXELS);
		InvalidateRect(hwnd, &clientRect, false);
		break;
	}
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
		firstPosition = clientRect.right;
		secondPosition = movableRect.right + offset;
		break;
	}

	if ((secondPosition - firstPosition) > -1) {
		return -10 * offset;
	}

	return offset;
}

void correctChordsMouse() {
	if (movableRect.left <= clientRect.left + 3) dragMovableRect(RIGHT, 20);
	if (movableRect.top <= clientRect.top + 3) dragMovableRect(DOWN, 20);
	if (movableRect.right >= clientRect.right - 3) dragMovableRect(LEFT, 20);
	if (movableRect.bottom >= clientRect.bottom - 3) dragMovableRect(UP, 20);
}