#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = _T("Ropmode");

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
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int sx, sy, oldx, oldy;
	static BOOL bNowDraw = FALSE;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		oldx = sx;
		oldy = sy;
		bNowDraw = TRUE;
		return 0;
	case WM_MOUSEMOVE:
		if (bNowDraw) {
			hdc = GetDC(hWnd);
			SetROP2(hdc, R2_NOT); //�� �ڵ带 �����ϰ� �����ϸ� ���� �̻��ϰ� �׷����� Ȯ��.
			SelectObject(hdc, GetStockObject(WHITE_PEN)); //67��°�� �ڵ�� ��������.
			MoveToEx(hdc, sx, sy, NULL);
			LineTo(hdc, oldx, oldy);
			oldx = LOWORD(lParam);
			oldy = HIWORD(lParam);
			SelectObject(hdc, GetStockObject(BLACK_PEN)); //SetRop2���� �� �� �ڵ� Ȱ��ȭ ��Ű��, ���� �� ���� ���� ���׷��� ��� ����� ���� Ȯ��.
			MoveToEx(hdc, sx, sy, NULL);
			LineTo(hdc, oldx, oldy);
			ReleaseDC(hWnd, hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		bNowDraw = FALSE;
		hdc = GetDC(hWnd);
		MoveToEx(hdc, sx, sy, NULL);
		LineTo(hdc, oldx, oldy);
		return 0;
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}