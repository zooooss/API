#include <windows.h>
#include <tchar.h>

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpszCmdParam, _In_ int nCmdShow) {
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	SOCKET s;
	SOCKADDR_IN addr = { 0 };

	s = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(s, (LPSOCKADDR)&addr, sizeof(addr));
	
	if (listen(s, 5) == -1)
		return 0;

	do {
		SOCKADDR_IN c_addr;
		int size = sizeof(c_addr);
		accept(s, (LPSOCKADDR)&c_addr, &size);
	} while (MessageBox(NULL,
		_T("클라이언트 접속을 확인했습니다. 서버를 종료하시겠습니까?"),
		_T("winsock"), MB_YESNO) == IDNO);

	closesocket(s);
	WSACleanup();
	return 1;
}