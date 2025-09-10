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
		Sleep(1);
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
	while (true)
	{
		std::cout << "Peer IP: ";
		std::cin >> peerAddress;
		try
		{
			peer->peerConnect(peerAddress);
			std::cout << "Connected!" << std::endl;
			break;
		}
		catch (const std::exception&)
		{
			peer->peerDisconnect();
			std::cout << "Connection Failed Try A Real IP" << std::endl;
		}
	}
}