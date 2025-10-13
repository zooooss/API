#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MajorDlgProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = _T("DlgCheck");

BOOL bKorean;
enum tag_major{MANAGE,KOREAN,ENGLISH,LAW}Major;
LPCTSTR arMajor[] = { _T("경영학과"),_T("국문학과"),_T("영문학과") ,_T("법학과") };

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

BOOL CALLBACK MajorDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_INITDIALOG:
		CheckDlgButton(hDlg, IDC_NATIONALITY, bKorean ? BST_CHECKED : BST_UNCHECKED);
		CheckRadioButton(hDlg, IDC_MAJOR1, IDC_MAJOR4, IDC_MAJOR1 + Major);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			bKorean = (IsDlgButtonChecked(hDlg, IDC_NATIONALITY) == BST_CHECKED);
			if (IsDlgButtonChecked(hDlg, IDC_MAJOR1) == BST_CHECKED)Major = MANAGE;
			if (IsDlgButtonChecked(hDlg, IDC_MAJOR2) == BST_CHECKED)Major = KOREAN;
			if (IsDlgButtonChecked(hDlg, IDC_MAJOR3) == BST_CHECKED)Major = ENGLISH;
			if (IsDlgButtonChecked(hDlg, IDC_MAJOR4) == BST_CHECKED)Major = LAW;
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:
		bKorean = TRUE;
		Major = KOREAN;
		return 0;
	case WM_LBUTTONDOWN:
		if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)MajorDlgProc) == IDOK) {
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		_stprintf_s(str, _T("국적=%s"), bKorean ? _T("한국인") : _T("외국인"));
		TextOut(hdc, 10, 10, str,_tcslen(str));
		_stprintf_s(str, _T("전공=%s"), arMajor[Major]);
		TextOut(hdc, 10, 30, str, _tcslen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
