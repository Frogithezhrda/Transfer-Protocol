#pragma once
#include <Windows.h>
#include <string>
#include <thread>
#include "ConnectionException.hpp"
#include <vector>

constexpr const int PORT = 6767;
constexpr const char* HOST = "127.0.0.1";

class Peer
{
public:
	Peer();
	~Peer();
	void peerConnect(const std::string& hostIP) const;
	void peerDisconnect() const;
private:
	SOCKET m_serverSocket;
	SOCKET m_clientSocket;

	void initializeServerSocket();
	void createSocket();
	void bindAndListen();
	void handleRequests();
	void handleNewClient(const SOCKET& clientSocket);
	SOCKET acceptClient() const;
};