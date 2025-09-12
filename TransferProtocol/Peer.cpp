#include "Peer.h"

Peer::Peer()
{
	options();
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

void Peer::options()
{
	int choice = 0;
	std::cout << "Choose (0 for client, 1 for server): ";
	std::cin >> choice;
	if (!choice)
	{
		peerInput();
	}
	else
	{
		createSocket(m_serverSocket);
		handleRequests();
	}
}

void Peer::peerInput()
{
	std::string peerAddress = "";
	std::string fileName = "";
	while (true)
	{
		std::cout << "Peer IP: ";
		std::cin >> peerAddress;
		try
		{
			createSocket(m_clientSocket);
			peerConnect(peerAddress);
			std::cout << "Connected!" << std::endl;
			std::cout << "File Name: ";
			std::cin >> fileName;
			FileTransfer transfer(fileName, std::make_shared<SOCKET>(getClientSocket()));
			transfer.startTransfer();
			std::cout << "Transfer Done!" << std::endl;
			options();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void Peer::initializeServerSocket()
{
}

void Peer::createSocket(SOCKET& sock)
{
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
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
	long i = 0;
	std::cout << "File Name: ";
	std::cin >> fileName;
	FileTransfer transfer(fileName, std::make_shared<SOCKET>(clientSocket));
	while (transfer.receiveNextChunk())
	{
		i++;
		if (i % 4000 == 0)
		{
			std::cout << "Current Chunks: " << transfer.getChunks()->getFileChunks().size();
			transfer.getChunks()->writeChunksToFile(fileName);
		}
	}
	transfer.getChunks()->writeChunksToFile(fileName);
	std::cout << "Got The File! " << fileName << std::endl;
	options();
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