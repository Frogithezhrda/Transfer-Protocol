#pragma comment (lib, "ws2_32.lib")
#include "WinInitializer.hpp"
#include "Peer.h"


void main(void)
{
	WSAInitializer wsa;
	try
	{
		Peer peer;
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what();
	}
}