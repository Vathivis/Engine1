#include <WS2tcpip.h>

#include "UDPServer.h"

#include "Engine1/Log.h"

#pragma comment (lib, "ws2_32.lib")


std::mutex mu, mu2;

UDPServer::UDPServer() {

	//TODO: can be done only once, probably doesnt matter with server, but it does with client || or maybe not actually???
	this->initServer();

	m_in = socket(AF_INET, SOCK_DGRAM, 0);
	m_serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	m_serverHint.sin_family = AF_INET;
	m_serverHint.sin_port = htons(54000);		//konverze z little do big endian

	if (bind(m_in, (sockaddr*)& m_serverHint, sizeof(m_serverHint)) == SOCKET_ERROR) {
		E1_ERROR("Can't bind socket {0}", WSAGetLastError());
		return;
	}

	m_clientLength = sizeof(m_client);
	ZeroMemory(&m_client, m_clientLength);

	u_long mode = 1;
	ioctlsocket(m_in, FIONBIO, &mode);

}

void UDPServer::setState(bool state) {
	mu.lock();
	m_msgPending = state;
	mu.unlock();
}

void UDPServer::onUpdate() {

	while (true) {

		if (!m_msgPending) {
			mu.lock();
			ZeroMemory(m_buffer, 1024);
			//m_buffer[0] = '+';
			int bytesIn = recvfrom(m_in, m_buffer, 1024, 0, (sockaddr*)& m_client, &m_clientLength);
			mu.unlock();
			if (bytesIn == SOCKET_ERROR) {
				//E1_ERROR("Error recieving from client {0}", WSAGetLastError());
				//return;
				continue;
			}


			ZeroMemory(&m_clientIP, 256);

			inet_ntop(AF_INET, &m_client.sin_addr, m_clientIP, 256);
			E1_INFO("message recieved from {0} : {1}", m_clientIP, m_buffer);

			mu2.lock();
			m_msgPending = true;
			mu2.unlock();
		}
		//else
			//E1_WARN("Message Pending");

	}

}

void UDPServer::initServer() {

	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &m_data);
	if (wsOk) {
		E1_ERROR("Can't start winsock {0}", wsOk);
		return;
	}

}

void UDPServer::stopServer() {

	closesocket(m_in);
	WSACleanup();
}
