#include <iostream>
#include "../libs/tcpsock.hpp"
#include "../libs/env.hpp"
using namespace std;

int StoI(const wchar_t* str)
{
	int int32 = 0;
	for (int i = 0; str[i] != L'\0'; i++)
		int32 = (int32 * 10) + str[i] - L'0';
	return int32;
}

int _main(void)
{
	WSA wsa;
	wsa.start();
	ENV env;
	const wchar_t* ip = env[L"IP"];
	const wchar_t* port = env[L"PORT"];

	wcout << "ip: " << ip << "\nport: " << port << '\n';
	TCP listener;
	listener << address(ip, StoI(port));

	listener.close();
	delete[] ip;
	delete[] port;

	return 0;
}