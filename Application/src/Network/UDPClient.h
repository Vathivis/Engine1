#pragma once

#include <string>


class UDPClient {
private:

	WSADATA m_data;
	SOCKET m_out;
	sockaddr_in m_server;

public:

	UDPClient();

	void send(const std::string& message);

	void initClient();
	void stopClient();


};