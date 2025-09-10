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
	m_file.seekg(0, std::ios::end);
	fileSize = m_file.tellg();
	m_file.seekg(0, std::ios::beg);
	for (int i = 0; i < (fileSize / BYTE_COUNT) + 1; i++)
	{
		sendNextChunk();
	}
	closesocket(*m_clientSocket.get());
}

void FileTransfer::sendNextChunk()
{
	Chunk chunk = m_chunks.serialize(m_file);
	sendChunk(chunk);
}
void FileTransfer::sendChunk(Chunk chunk)
{
	std::string resultString = std::string(chunk.chunk.begin(), chunk.chunk.end());
	if (send(*m_clientSocket.get(), resultString.c_str(), chunk.chunkSize, 0) == SOCKET_ERROR)
	{
		throw TransferException("Could Not Send Data.");
	}
	std::cout << "Sended Message." << std::endl;
}

bool FileTransfer::receiveNextChunk()
{
	unsigned char data[BYTE_COUNT] = { 0 };
	long bytesReceived = 0;

	bytesReceived = recv(*m_clientSocket.get(), reinterpret_cast<char*>(data), BYTE_COUNT, 0);
	if (0 >= bytesReceived)
	{
		return false;
	}
	std::vector<char> chunkBytes(data, data + bytesReceived);
	m_chunks.deserialize(chunkBytes);
	return true;
}

std::shared_ptr<FileChunk> FileTransfer::getChunks() const
{
	return std::make_shared<FileChunk>(m_chunks);
}
