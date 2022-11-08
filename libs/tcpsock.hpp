#ifndef _TCPSOCK_H_
#define _TCPSOCK_H_

#pragma comment(lib, "Ws2_32")
#include <Ws2tcpip.h>
#include <queue>
#include <mutex>
#include <thread>
#include <cstdio>
#include <memory>

// address(L"Your Ip", Your Port)
class address
{
private:
	sockaddr_in addr;
	socklen_t addr_len;

public:
	address();
	address(const WCHAR* ip, int port);

	friend class TCP;

};

class TCP
{
private:
	static void error(const char* msg);
	static void warning(const char* msg);

protected:
	SOCKET h_sock;
	address addr_info;
	std::mutex m_tcpmutex;
	int backlog;

public:
	TCP();
	TCP(SOCKET);
	TCP(const TCP&);
	TCP(const address&);
	TCP(SOCKET, const address&);

	TCP& operator =(const TCP&);
	void operator >>(const address& addr_info);
	void operator <<(const address& addr_info);
	void operator >>(TCP&);

	void close();
	void non_blocking();
	void blocking();
	void sd_receive();
	void sd_send();
	void sd_both();
	int read(char* buff, int buff_len);
	int write(const char* msg, int msg_len);
	static int getErrorCode();
	bool sock_check();
	void set_backlog(int);
	int getopt(int level, int optname, char* optval, int * optlen);
	int setopt(int level, int optname, const char* optval, int optlen);

	friend class address;
	friend class WSA;
};

class WSA
{
protected:
	bool check;

public:
	WSADATA wsaData;

public:
	WSA();
	~WSA();

	void start();
	void stop();
};

#endif