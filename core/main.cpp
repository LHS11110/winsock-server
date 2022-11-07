#include <iostream>
#include "../libs/tcpsock.hpp"
using namespace std;

int StoI(const char* str)
{
	const size_t strSize = strlen(str) + 1;
	int int32 = 0;
	for (int i = 0; i < strSize; i++)
		int32 = (int32 * 10) + str[i] - '0';
	return int32;
}

const wchar_t* GetWC(const char* c)
{
	size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs_s(&cSize, wc, cSize, c, cSize);

	return wc;
}

int main(void)
{
	wsaManager manager;
	manager.start();
	TCP listener;
	const wchar_t* ip = GetWC(getenv("IP"));
	listener << address(ip, StoI(getenv("PORT")));
	delete[] ip;
	TCP clnt;
	listener >> clnt;
	clnt.write("Hi!", 3);
	clnt.close();
	listener.close();

	return 0;
}