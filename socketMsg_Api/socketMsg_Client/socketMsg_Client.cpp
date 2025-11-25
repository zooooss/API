#include <windows.h>
#include <tchar.h>

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpszCmdParam, _In_ int nCmdShow) {
	WSADATA wsadata;
	SOCKET s;
	SOCKADDR_IN addr = { 0 };

	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(s,(LPSOCKADDR)&addr,sizeof(addr))==-1)
		return 0;

	send(s, "æ»≥Á«œººø‰ Server!", 19, 0);
	closesocket(s);
	WSACleanup();
	return 0;
}