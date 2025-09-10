#pragma comment (lib, "ws2_32.lib")
#include "WinInitializer.hpp"
#include "Peer.h"

void peerInput(Peer* peer);

void main(void)
{
	WSAInitializer wsa;
	try
	{
		Peer peer;
		peerInput(&peer);
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what();
	}
}

void peerInput(Peer* peer)
{
	std::string peerAddress = "";
	std::string fileName = "";
	while (true)
	{
		std::cout << "Peer IP: ";
		std::cin >> peerAddress;
		try
		{
			peer->peerConnect(peerAddress);
			std::cout << "Connected!" << std::endl;
			std::cout << "File Name: ";
			std::cin >> fileName;
			FileTransfer transfer = FileTransfer(fileName, std::make_shared<SOCKET>(peer->getClientSocket()));
			transfer.startTransfer();
			std::cout << "Transfer Done!" << std::endl;
		}
		catch (const std::exception&)
		{
			std::cout << "Connection Failed Try A Real IP" << std::endl;
		}
	}
}