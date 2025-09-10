#include "Peer.h"

Peer::Peer()
{
	createSocket();
	initializeServerSocket();
}

Peer::~Peer()
{
	try
	{
		closesocket(this->m_serverSocket);
	}
	catch (...) {}
}

void Peer::peerConnect(const std::string& hostIP) const
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = inet_addr(hostIP.c_str());    // when there are few ip's for the machine. We will use always "INADDR_ANY"
	if (connect(m_clientSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) 
	{
		throw ConnectionException(__FUNCTION__ " - connect");
	}

	std::cout << "Connected to " << hostIP << ":" << PORT << std::endl;

}

void Peer::peerDisconnect() const
{
	closesocket(this->m_clientSocket);
}

SOCKET Peer::getClientSocket() const
{
	return m_clientSocket;
}

void Peer::initializeServerSocket()
{
	std::thread serverThread = std::thread(&Peer::handleRequests, this);
	serverThread.detach();
}

void Peer::createSocket()
{
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET || m_clientSocket == INVALID_SOCKET)
	{
		//getting function name and socket error
		throw ConnectionException(__FUNCTION__);
	}
}

void Peer::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	//connecting between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		throw ConnectionException(__FUNCTION__ " - bind");
	}

	//listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw ConnectionException(__FUNCTION__ " - listen");
	}
	std::cout << "Listening on port " << PORT << std::endl;
}

void Peer::handleRequests()
{
	//std::vector<std::thread> connections;
	try
	{
		bindAndListen();
		// and add then to the list of handlers
		//getting the sockets
		SOCKET clientSocket = acceptClient();
		//putting the threads
		//connections.emplace_back(std::thread(&Peer::handleNewClient, this, clientSocket));
		handleNewClient(clientSocket);
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what();
	}
}

void Peer::handleNewClient(const SOCKET& clientSocket)
{
	std::string fileName = "";
	std::cout << "File Name: ";
	std::cin >> fileName;
	FileTransfer transfer = FileTransfer(fileName, std::make_shared<SOCKET>(clientSocket));
	while (transfer.receiveNextChunk());
}

SOCKET Peer::acceptClient() const
{
	// this accepts the client and create a specific socket from server to this client
	// the process will not continue until a client connects to the server
	SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
		throw ConnectionException(__FUNCTION__);
	}

	std::cout << "Client accepted. Server and client can speak" << std::endl;
	// the function that handle the conversation with the client
	return clientSocket;
}