#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpszCmdParam, _In_ int nCmdShow) {
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	SOCKET s;
	SOCKADDR_IN addr = { 0 };

	TCHAR message[300]{};

	s = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(s, (LPSOCKADDR)&addr, sizeof(addr));

	if (listen(s, 5) == -1)
		return 0;
	
	do
	{
		SOCKADDR_IN c_addr;
		char buffer[100]{};		// 클라이언트가 보낸 메시지를 저장하기 위한 문자열 배열
#ifdef _UNICODE
		TCHAR wbuffer[100]{};	// 유니코드일 경우 저장할 문자열 배열
#endif
		int msgLeng;			// 보낸 메시지의 길이 저장할 변수
		int size = sizeof(c_addr);
		// accept( ) 함수의 리턴값은 소켓, cs를 이용하여 소켓 관리
		SOCKET cs = accept(s, (LPSOCKADDR)&c_addr, &size);	// 클라이언트의 접속을 받음
		msgLeng = recv(cs, buffer, 100, 0);
		buffer[msgLeng]; // 기존코드 buffer[msgLeng] = NULL;
#ifdef _UNICODE
		msgLeng = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, wbuffer, msgLeng);
		wbuffer[msgLeng] = NULL;
		_stprintf_s(message, _T("[U]클라이언트 메시지: % s, 서버를 종료하시겠습니까?"), wbuffer);
#else
		_stprintf_s(message, _T("[M]클라이언트 메시지: % s, 서버를 종료하시겠습니까?"), buffer);
#endif
	} while (MessageBox(NULL, message, _T("Server 메시지"), MB_YESNO) == IDNO);

	closesocket(s);
	WSACleanup;
	return 1;
}