#pragma once
#include "FileChunk.h"
#include <WinSock2.h>
#include "TransferException.hpp"

class FileTransfer
{
public:
	FileTransfer(const std::string& fileName, const std::shared_ptr<SOCKET> socket);
	~FileTransfer();
	void startTransfer();
	void sendNextChunk();
	bool receiveNextChunk();

private:
	FileChunk m_chunks;
	std::ifstream m_file;
	std::shared_ptr<SOCKET> m_clientSocket;

	void sendChunk(Chunk message);

};