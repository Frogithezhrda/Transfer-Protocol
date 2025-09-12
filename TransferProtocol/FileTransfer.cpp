#include "FileTransfer.h"

FileTransfer::FileTransfer(const std::string& fileName, const std::shared_ptr<SOCKET> socket)
{
	m_file = std::ifstream(fileName, std::ios::binary);
	m_clientSocket = socket;
}

FileTransfer::~FileTransfer()
{
	m_file.close();
}

void FileTransfer::startTransfer()
{
	long fileSize = 0;
	clock_t start = clock();
	m_file.seekg(0, std::ios::end);
	fileSize = m_file.tellg();
	m_file.seekg(0, std::ios::beg);
	while (!m_file.eof())
	{
		sendNextChunk();
	}
	clock_t end = clock();
	double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
	std::cout << "Sended Message. Ran For - " << time << "s" << std::endl;
	closesocket(*m_clientSocket.get());
}

void FileTransfer::sendNextChunk()
{
	Chunk chunk = m_chunks.serialize(m_file);
	sendChunk(chunk);
}

void FileTransfer::sendChunk(Chunk chunk)
{
	if (send(*m_clientSocket.get(), chunk.chunk.data(), chunk.chunkSize, 0) == SOCKET_ERROR)
	{
		throw TransferException("Could Not Send Data.");
	}
}

bool FileTransfer::receiveNextChunk()
{
	unsigned char* data = new unsigned char[BYTE_COUNT];
	long bytesReceived = 0;

	bytesReceived = recv(*m_clientSocket.get(), reinterpret_cast<char*>(data), BYTE_COUNT, 0);
	if (0 >= bytesReceived)
	{
		return false;
	}
	std::vector<char> chunkBytes(data, data + bytesReceived);
	delete[] data;
	m_chunks.deserialize(chunkBytes);
	return true;
}

FileChunk* FileTransfer::getChunks()
{
	return &m_chunks;
}
