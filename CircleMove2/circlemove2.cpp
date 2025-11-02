#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
POINT CenterPoint(RECT&);
void DrawObject(HDC, RECT&, COLORREF, int);
void DrawObject(HDC, RECT&, COLORREF, COLORREF, int);

HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = _T("Circle Move2");

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		400, 160, 800, 700,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

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

	static RECT ballR;
	static int alphaX;
	static int alphaY;
	static COLORREF ballCol;
	static BOOL isVisible;

	POINT p;
	RECT clientR;

	switch (iMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &clientR);
		p = CenterPoint(clientR);
		SetRect(&ballR, p.x - 7, p.y - 7, p.x + 7, p.y + 7);

		alphaX = 1;
		alphaY = 0;
		ballCol = RGB(255, 0, 0);
		isVisible = TRUE;

		SetTimer(hWnd, 1, 10, NULL);   // 이동용 타이머
		SetTimer(hWnd, 2, 200, NULL);  // 깜빡임용 타이머
		return 0;
	case WM_TIMER:

		GetClientRect(hWnd, &clientR);

		if (wParam == 1) {
			// 이동
			OffsetRect(&ballR, 2 * alphaX, 2 * alphaY);

			// 벽 충돌 감지
			if (ballR.left <= clientR.left || ballR.right >= clientR.right)
				alphaX = -alphaX;
			if (ballR.top <= clientR.top || ballR.bottom >= clientR.bottom)
				alphaY = -alphaY;

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 2) {
			// 깜빡임 토글
			isVisible = !isVisible;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (isVisible)
			DrawObject(hdc, ballR, ballCol, 1);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			alphaX = -1; alphaY = 0;
			break;
		case VK_RIGHT:
			alphaX = 1; alphaY = 0;
			break;
		case VK_UP:
			alphaX = 0; alphaY = -1;
			break;
		case VK_DOWN:
			alphaX = 0; alphaY = 1;
			break;
		}
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

POINT CenterPoint(RECT& r) {
	POINT p;
	p.x = (r.left + r.right) / 2;
	p.y = (r.top + r.bottom) / 2;
	return p;
}

void DrawObject(HDC hdc, RECT& r, COLORREF color, int type) {
	DrawObject(hdc, r, color, color, type);
}

void DrawObject(HDC hdc, RECT& r, COLORREF penC, COLORREF brushC, int type) {
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	hPen = CreatePen(PS_SOLID, 1, penC);
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(brushC);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	switch (type) {
	case 0:
		Rectangle(hdc, r.left, r.top, r.right, r.bottom);
		break;
	case 1:
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		break;
	}
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}