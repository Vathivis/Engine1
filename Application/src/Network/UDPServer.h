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
	bool m_stopThread = false;

public:

	UDPServer();
	~UDPServer();

	//getters
	inline std::string getBuffer() const { std::string s(m_buffer); return s; }
	inline bool getState() const { return m_msgPending; }

	//setters
	void setState(bool state);
	inline void stopThread() { m_stopThread = true; }

	void onUpdate();

	void initServer();
	void stopServer();


};

