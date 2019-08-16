#pragma once

#include <string>

class UDPServer {
private:

	WSADATA m_data;
	SOCKET m_in;
	sockaddr_in m_serverHint;
	sockaddr_in m_client;

	char m_buffer[1024];
	char m_clientIP[256];
	int m_clientLength;

	bool m_msgPending = false;

public:

	UDPServer();

	//getters
	inline std::string getBuffer() const { std::string s(m_buffer); return s; }
	inline bool getState() const { return m_msgPending; }

	//setters
	void setState(bool state);

	void onUpdate();

	void initServer();
	void stopServer();


};

