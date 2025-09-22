#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = _T("Key");

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd; // Store hWnd information in a global variable hWndMain!

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message); //키보드에 문자정보를 담을 때에 꼭 필요하다
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x = 100;
	static int y = 100;
	/*
	static TCHAR str[256] = _T("");
	int len;
	*/

	switch (iMessage) {
		/*
	case WM_CHAR:
		if (wParam == 32) {
			str[0] = 0;
		}
		else {
			len = _tcslen(str);
			str[len] = wParam;
			str[len + 1] = 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 50, 50, str, _tcslen(str));
		EndPaint(hWnd, &ps);
		return 0;
				*/
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			x -= 8;
			break;
		case VK_RIGHT:
			x += 8;
			break;
		case VK_UP:
			y -= 8;
			break;
		case VK_DOWN:
			y += 8;
			break;
		}
		InvalidateRect(hWnd,NULL,FALSE); //얘가 윈도우메세지페인트를 강제로 발생시킴

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, x, y, _T("A"), 1);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

// get the length of the string temp
// num = _tcslen(temp);


// Write to string 1
// #include <stdio.h> file required
// _stprintf_s(temp, _T("Cown Down: %2d"), Count);


// Write to string 2
// _tcscpy_s(temp, _T("Compatibility support"));