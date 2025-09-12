#pragma once
#include <string>
#include <thread>
#include "ConnectionException.hpp"
#include <vector>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "FileTransfer.h"
#include <Windows.h>

constexpr const int PORT = 6767;
constexpr const char* HOST = "127.0.0.1";

class Peer
{
public:
	Peer();
	~Peer();
	void peerConnect(const std::string& hostIP) const;
	void peerDisconnect() const;
	SOCKET getClientSocket() const;
private:
	SOCKET m_serverSocket;
	SOCKET m_clientSocket;

	void options();
	void peerInput();
	void initializeServerSocket();
	void createSocket(SOCKET& sock);
	void bindAndListen();
	void handleRequests();
	void handleNewClient(const SOCKET& clientSocket);
	SOCKET acceptClient() const;
};