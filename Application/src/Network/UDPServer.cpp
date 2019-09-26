#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <sstream>

#include "UDPServer.h"

#include "Engine1/Log.h"

#pragma comment (lib, "ws2_32.lib")


std::mutex mu, mu2;

UDPServer::UDPServer() {

	//TODO: can be done only once, probably doesnt matter with server, but it does with client || or maybe not actually???
	/*this->initServer();
	E1_INFO("Server initialized");

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
	ioctlsocket(m_in, FIONBIO, &mode);*/

}

UDPServer::~UDPServer() {
	this->stopServer();
}

/*std::string UDPServer::getBuffer() const {
	std::string s(m_buffer1);
	return s;
}*/

void UDPServer::setState(bool state) {
	mu.lock();
	m_msgPending = state;
	mu.unlock();
}

void UDPServer::onUpdate() {

	// Open serial port
	HANDLE serialHandle;

	char handleName[5] = "COM0";
	for (int i = 0; i < 10; ++i) {
		handleName[3] = 48 + i;
		wchar_t hn[5];
		std::mbstowcs(hn, handleName, 6);
		LPCWSTR ptr = hn;
		serialHandle = CreateFile(ptr, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (serialHandle != INVALID_HANDLE_VALUE)
			break;
	}



	// Do some basic settings
	DCB serialParams = { 0 };
	serialParams.DCBlength = sizeof(serialParams);

	GetCommState(serialHandle, &serialParams);
	serialParams.BaudRate = CBR_19200;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;
	SetCommState(serialHandle, &serialParams);

	// Set timeouts
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 50;
	timeout.ReadTotalTimeoutConstant = 50;
	timeout.ReadTotalTimeoutMultiplier = 10;
	timeout.WriteTotalTimeoutConstant = 50;
	timeout.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(serialHandle, &timeout)) {
		//error occureed. Inform user
		char lastError[1024];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)lastError, 1024, NULL);
		std::cout << lastError << std::endl;
	}

	//NOTE: ma = anchor to anchor ranging report
	//      mr = tag to anchor ranging report (raw)
	//      mc = tag to anchor ranging report (range bias corrected)
	const int n = 64;
	while (!m_stopThread) {

		//needs COM port synchronization, probably wont be needed in the future since the communication will be provided by ethernet
		//ethernet communication is the commented out part, for now works only with local host, not tested otherwise

		char szBuff[n + 1] = { 0 };
		DWORD dwBytesRead = 0;
		if (!ReadFile(serialHandle, szBuff, n, &dwBytesRead, NULL)) {
			//error occurred. Report to user.
			std::cout << "err" << std::endl;
			continue;
		}

		std::string x(szBuff);
		std::string y;
		remove_copy(x.begin(), x.end(), std::back_inserter(y), '\n' || '\r');

		std::stringstream str;
		str << y;

		if (y == "\n")
			continue;

		std::string header, end;
		int num1, num2, num3, num4;
		int pos4;

		str >> header >> std::hex >> num1 >> m_pos1 >> m_pos2 >> m_pos3 >> pos4 >> num2 >> num3 >> num4 >> end;

		if (header != "mc")
			continue;

		std::cout << header << " " << num1 << " " << m_pos1 << " " << m_pos2 << " " << m_pos3 << std::endl;



		/*if (!m_msgPending) {
			mu.lock();
			ZeroMemory(m_buffer, 1024);

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
		}*/
		//else
			//E1_WARN("Message Pending");




		/*if (m_buffer1Empty) {
			mu.lock();
			ZeroMemory(m_buffer1, 1024);

			int bytesIn = recvfrom(m_in, m_buffer1, 1024, 0, (sockaddr*)& m_client, &m_clientLength);
			mu.unlock();
			if (bytesIn == SOCKET_ERROR) {
				//E1_ERROR("Error recieving from client {0}", WSAGetLastError());
				//return;
				continue;
			}


			ZeroMemory(&m_clientIP, 256);

			inet_ntop(AF_INET, &m_client.sin_addr, m_clientIP, 256);
			E1_INFO("message recieved from {0} : {1}", m_clientIP, m_buffer1);

			mu2.lock();
			m_buffer1Empty = false;
			mu2.unlock();
		}
		else if (m_buffer2Empty) {
			mu.lock();
			ZeroMemory(m_buffer2, 1024);

			int bytesIn = recvfrom(m_in, m_buffer2, 1024, 0, (sockaddr*)& m_client, &m_clientLength);
			mu.unlock();
			if (bytesIn == SOCKET_ERROR) {
				//E1_ERROR("Error recieving from client {0}", WSAGetLastError());
				//return;
				continue;
			}


			ZeroMemory(&m_clientIP, 256);

			inet_ntop(AF_INET, &m_client.sin_addr, m_clientIP, 256);
			E1_INFO("message recieved from {0} : {1}", m_clientIP, m_buffer2);


			mu2.lock();
			m_buffer2Empty = false;
			mu2.unlock();
		}
		else {

		}*/


	}

	CloseHandle(serialHandle);

}

void UDPServer::initServer() {

	/*WORD version = MAKEWORD(2, 2);
	int wsOk = WSAStartup(version, &m_data);
	if (wsOk) {
		E1_ERROR("Can't start winsock {0}", wsOk);
		return;
	}*/

}

void UDPServer::stopServer() {

	/*closesocket(m_in);
	WSACleanup();*/
}
