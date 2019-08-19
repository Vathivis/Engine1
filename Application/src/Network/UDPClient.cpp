#include <WS2tcpip.h>	

#include "UDPClient.h"

#include "Engine1/Log.h"

#pragma comment (lib, "ws2_32.lib")

UDPClient::UDPClient() {

	m_server.sin_family = AF_INET;
	m_server.sin_port = htons(54000);

	inet_pton(AF_INET, "127.0.0.1", &m_server.sin_addr);

	m_out = socket(AF_INET, SOCK_DGRAM, 0);

}

void UDPClient::send(const std::string& message) {

	int sendOk;

	sendOk = sendto(m_out, message.c_str(), message.size() + 1, 0, (sockaddr*)& m_server, sizeof(m_server));


	if (sendOk == SOCKET_ERROR) {
		E1_ERROR("message not sent ", WSAGetLastError());
	}
	
}

void UDPClient::initClient() {

	WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &m_data);
	if (wsOk) {
		E1_ERROR("Can't start winsock {0}", wsOk);
		return;
	}

}

void UDPClient::stopClient() {
	closesocket(m_out);
	WSACleanup();
}
