// VS_2019/2022 ���� seconds.cpp

#include <windows.h>
#include <TCHAR.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = _T("TextOut2");

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance
	, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	hWndMain = hWnd; // hWnd ������ ���������� ����!	

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
	/*
	RECT rt = { 50,20,350,220 };
	LPCTSTR str = _T("���� �����ϴ�. �ƾ� ����ϴ� ���� ���� �����ϴ�. Ǫ�� �����")
		_T("��ġ�� ��ǳ���� ���� ���Ͽ� �� ���� ���� �ɾ ���� ��ġ�� �����ϴ�.")
		_T("Ȳ���� �ɰ��� ���� ������ �� �翡�� ������ Ƽ���� �Ǿ� �Ѽ��� ��ǳ��")
		_T("���ư����ϴ�.");
	*/

	switch (iMessage) {
	/*
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		TextOut(hdc, 100, 100, _T("Beautiful Korea"), 15);
		ReleaseDC(hWnd, hdc);
		return 0;
		*/
	case WM_PAINT:
		/*
		hdc = BeginPaint(hWnd, &ps);
		DrawText(hdc, str, _tcslen(str), &rt, DT_CENTER | DT_WORDBREAK);
		*/
		/*
		SetTextAlign(hdc, TA_UPDATECP);
		//SetTextAlign(hdc, TA_CENTER);
		TextOut(hdc, 100, 60, _T("Beautiful Korea"), 15);
		TextOut(hdc, 100, 80, _T("is My"), 5);
		*/
		hdc = BeginPaint(hWnd, &ps);
		SetPixel(hdc, 10, 10, RGB(255, 0, 0));
		MoveToEx(hdc, 50, 50, NULL);
		LineTo(hdc, 300, 90);
		Rectangle(hdc, 50, 100, 200, 180);
		Ellipse(hdc, 220, 100, 400, 200);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		MessageBox(hWnd, _T("Click Mouse Left Button!"), _T("Message Box!!"), MB_OK);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}