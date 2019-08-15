#pragma once

class UDPServer {
private:

	WSADATA m_data;
	SOCKET m_in;
	sockaddr_in m_serverHint;
	sockaddr_in m_client;

	char m_buffer[1024];
	char m_clientIP[256];
	int m_clientLength;

public:

	UDPServer();

	void onUpdate();

	void initServer();
	void stopServer();


};

