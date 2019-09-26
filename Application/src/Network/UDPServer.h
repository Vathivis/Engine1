#pragma once

#include <string>
#include "glm/glm.hpp"


class UDPServer {
private:

	WSADATA m_data;
	SOCKET m_in;
	sockaddr_in m_serverHint;
	sockaddr_in m_client;

	//temporary
	char m_buffer[1024];
	bool m_msgPending = false;

	char m_buffer1[1024];
	char m_buffer2[1024];
	char m_clientIP[256];
	int m_clientLength;

	bool m_buffer1Empty = true;
	bool m_buffer2Empty = true;
	bool m_stopThread = false;

	//COM port temporary variables
	int m_pos1, m_pos2, m_pos3;


public:

	UDPServer();
	~UDPServer();

	//getters
	inline std::string getBuffer() const { std::string s(m_buffer); return s; }
	//inline std::string getBuffer() const;
	inline bool getState() const { return m_msgPending; }
	inline glm::vec3 getDists() const { return glm::vec3(m_pos1, m_pos2, m_pos3); }		//comm port getter

	//setters
	void setState(bool state);
	inline void stopThread() { m_stopThread = true; }

	void onUpdate();

	void initServer();
	void stopServer();


};

