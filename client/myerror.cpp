#include <iostream>
#include <WinSock2.h>



void GetFormattedMessage(const char* ErrorText)
{
	char* Temp = new char[200];

	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);

	printf("WARNING:  %s Failed with the following error:%s/n", (char*)ErrorText, (char*)lpMsgBuf);


	return;
}