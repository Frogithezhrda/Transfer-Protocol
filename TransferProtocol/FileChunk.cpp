#include "FileChunk.h"

Chunk FileChunk::serialize(std::ifstream& file) const
{
	Chunk chunk;
	char* fileBytes = new char[BYTE_COUNT];
	if (file.read(fileBytes, BYTE_COUNT))
	{
		if (file.gcount() == 0)
		{
			delete[] fileBytes;
			throw FileException(__FUNCTION__);
		}
	}
	chunk.chunk = std::vector<char>(fileBytes, fileBytes + BYTE_COUNT);
	chunk.chunkSize = file.gcount();
	delete[] fileBytes;
	return chunk;
}

void FileChunk::deserialize(const std::vector<char>& chunkBytes)
{
	Chunk chunk;
	chunk.chunk = chunkBytes;
	chunk.chunkSize = chunkBytes.size();
	chunks.push_back(chunk);
}

void FileChunk::writeChunksToFile(const std::string& fileName)
{
	std::ofstream file(fileName, std::ios::binary | std::ios::app);
	if (!file) throw FileException("Cannot open file");

	for (const auto& chunk : chunks) 
	{
		file.write(chunk.chunk.data(), chunk.chunkSize);
	}
	file.close();
	chunks.clear();
}

std::vector<Chunk>& FileChunk::getFileChunks() 
{
	return this->chunks;
}
