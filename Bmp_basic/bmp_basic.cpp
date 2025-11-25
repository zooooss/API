#include <windows.h>
#include <tchar.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void TextPrint(HDC, int, int, TCHAR[]);

HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = _T("BMP BASIC");

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
700,300,560,580,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	//hWndMain=hWnd; // hWnd 정보도 전역변수에 저장!

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,
	WPARAM wParam, LPARAM lParam)
{
	static HDC hdc,mem1dc, mem2dc;
	PAINTSTRUCT ps;

	static HBITMAP hBit1,hBit2,oldBit1,oldBit2;
	static RECT rectView;
	static int yPos;
	TCHAR word[] = _T("대한민국 화이팅");

	switch (iMessage) {
	case WM_CREATE:
		yPos = -30;
		GetClientRect(hWnd, &rectView);
		SetTimer(hWnd,1,70,NULL);
		//hBitmap = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		hBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		return 0;
	case WM_TIMER:
		yPos += 5;
		if (yPos > rectView.bottom)
			yPos = -30;
		hdc = GetDC(hWnd);
		if (hBit1 == NULL)
			hBit1 = CreateCompatibleBitmap(hdc, 1024, 768);
		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);

		BitBlt(mem1dc, 0, 0, 1024, 768, mem2dc, 0, 0, SRCCOPY);
		SetBkMode(mem1dc, TRANSPARENT);
		TextPrint(mem1dc, 200, yPos, word);
		SelectObject(mem2dc, oldBit2);
		DeleteDC(mem2dc);
		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem1dc);
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
		GetClientRect(hWnd, &rectView);
		hdc = BeginPaint(hWnd, &ps);
		mem1dc = CreateCompatibleDC(hdc);
		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);

		BitBlt(hdc, 0, 0, 1024, 768, mem1dc, 0, 0, SRCCOPY);
		SelectObject(mem1dc, oldBit1);
		DeleteDC(mem2dc);

		/*BitBlt(hdc, 0, 0, 332, 240, memdc, 0, 0, SRCCOPY);*/
		//TextOut(hdc, 200, yPos, word, _tcslen(word));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		if (hBit1)
			DeleteObject(hBit1);
		DeleteObject(hBit2);
		KillTimer(hWnd, 1);
		DeleteDC(mem1dc);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void TextPrint(HDC hdc, int x, int y, TCHAR text[]) {
	int i, j;
	SetTextColor(hdc, RGB(255, 255, 255));
	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++)
			TextOut(hdc, x + i, y + j, text, _tcslen(text));
	}
	SetTextColor(hdc, RGB(0, 0, 0));
	TextOut(hdc, x, y, text, _tcslen(text));
}