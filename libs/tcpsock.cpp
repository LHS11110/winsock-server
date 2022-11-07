#include "tcpsock.hpp"

address::address()
{
	addr_len = sizeof(sockaddr_in);
	memset(&addr, 0, addr_len);
}

address::address(const WCHAR* ip, int port)
{
	addr_len = sizeof(sockaddr_in);
	memset(&addr, 0, addr_len);
	if (InetPtonW(AF_INET, (PCWSTR)(ip), &addr.sin_addr.S_un.S_addr) != 1)
		TCP::error("Address Error");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
}

void TCP::error(const char* msg)
{
	printf("TCP: %s\n", msg);
	exit(-1);
}

void TCP::warning(const char* msg)
{
	printf("TCP: %s\n", msg);
}

TCP::TCP()
	: h_sock(INVALID_SOCKET), backlog(0)
{
	addr_info = address();
	blocking();
}

TCP::TCP(SOCKET sock)
	: h_sock(sock), backlog(0)
{
	addr_info = address();
	blocking();
}

TCP::TCP(const TCP& __o)
	: h_sock(__o.h_sock), addr_info(__o.addr_info), backlog(0)
{
	blocking();
}

TCP::TCP(const address& _addr_info)
	: h_sock(INVALID_SOCKET), backlog(0)
{
	addr_info = _addr_info;
	blocking();
}

TCP::TCP(SOCKET sock, const address& _addr_info)
	: h_sock(sock), backlog(0)
{
	addr_info = _addr_info;
	blocking();
}

TCP& TCP::operator=(const TCP& _tcp)
{
	m_tcpmutex.lock();
	h_sock = _tcp.h_sock;
	addr_info = _tcp.addr_info;
	m_tcpmutex.unlock();
	return *this;
}

void TCP::operator>>(const address& _addr_info)
{
	m_tcpmutex.lock();
	if (h_sock == INVALID_SOCKET)
		h_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr_info = _addr_info;
	if (connect(h_sock, (sockaddr*)&addr_info.addr, addr_info.addr_len) == SOCKET_ERROR)
	{
		m_tcpmutex.unlock();
		error("Connect Error");
		return;
	}
	m_tcpmutex.unlock();
}

void TCP::operator<<(const address& _addr_info)
{
	m_tcpmutex.lock();
	if (h_sock == INVALID_SOCKET)
		h_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr_info = _addr_info;
	if (bind(h_sock, (sockaddr*)&addr_info.addr, addr_info.addr_len) == SOCKET_ERROR)
	{
		m_tcpmutex.unlock();
		error("Bind Error");
		return;
	}
	if (listen(h_sock, backlog) != 0)
	{
		m_tcpmutex.unlock();
		error("Listen Error");
		return;
	}
	if (backlog == 0)
		backlog = 5;
	m_tcpmutex.unlock();
}

void TCP::operator>>(TCP& _tcp)
{
	m_tcpmutex.lock();
	_tcp.h_sock = accept(h_sock, (sockaddr*)&_tcp.addr_info.addr, &_tcp.addr_info.addr_len);
	m_tcpmutex.unlock();
}

void TCP::close()
{
	m_tcpmutex.lock();
	if (h_sock == INVALID_SOCKET)
	{
		m_tcpmutex.unlock();
		return;
	}
	closesocket(h_sock);
	h_sock = INVALID_SOCKET;
	m_tcpmutex.unlock();
}

void TCP::non_blocking()
{
	m_tcpmutex.lock();
	if (h_sock == INVALID_SOCKET)
		h_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	u_long nonBlockingMode = 1;
	if (ioctlsocket(h_sock, FIONBIO, &nonBlockingMode) != 0)
		error("Non Blocking Mode Error");
	m_tcpmutex.unlock();
}

void TCP::blocking()
{
	m_tcpmutex.lock();
	if (h_sock == INVALID_SOCKET)
		h_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	u_long blockingMode = 0;
	if (ioctlsocket(h_sock, FIONBIO, &blockingMode) != 0)
		error("Blocking Mode Error\n");
	m_tcpmutex.unlock();
}

void TCP::sd_receive()
{
	m_tcpmutex.lock();
	shutdown(h_sock, SD_RECEIVE);
	m_tcpmutex.unlock();
}

void TCP::sd_send()
{
	m_tcpmutex.lock();
	shutdown(h_sock, SD_SEND);
	m_tcpmutex.unlock();
}

void TCP::sd_both()
{
	m_tcpmutex.lock();
	shutdown(h_sock, SD_BOTH);
	m_tcpmutex.unlock();
}

int TCP::read(char* buff, int buff_len)
{
	m_tcpmutex.lock();
	if (h_sock == INVALID_SOCKET)
	{
		m_tcpmutex.unlock();
		return -1;
	}
	int result = recv(h_sock, buff, buff_len, 0);
	m_tcpmutex.unlock();
	return result;
}

int TCP::write(const char* msg, int msg_len)
{
	m_tcpmutex.lock();
	if (h_sock == INVALID_SOCKET)
	{
		m_tcpmutex.unlock();
		return -1;
	}
	int result = send(h_sock, msg, msg_len, 0);
	m_tcpmutex.unlock();
	return result;
}

int TCP::getErrorCode()
{
	return WSAGetLastError();
}

bool TCP::sock_check()
{
	return h_sock != INVALID_SOCKET;
}

void TCP::backlog_set(int int32)
{
	m_tcpmutex.lock();
	backlog = int32;
	m_tcpmutex.unlock();
}

wsaManager::wsaManager()
	: wsaData(), check(false)
{

}

wsaManager::~wsaManager()
{
	if (check)
		WSACleanup();
}

void wsaManager::start()
{
	if (check)
	{
		TCP::warning("Call this function only once");
		return;
	}
	check = true;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		TCP::error("WSAStartup Error\n");
}

void wsaManager::stop()
{
	check = false;
	WSACleanup();
}