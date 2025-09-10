#include "FileChunk.h"

Chunk FileChunk::serialize(std::ifstream& file) const
{
	Chunk chunk;
	char* fileBytes = new char[BYTE_COUNT];
	if (file.read(fileBytes, BYTE_COUNT))
	{
		delete[] fileBytes;
		throw FileException(__FUNCTION__);
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
	std::ofstream file(fileName, std::ios::binary);
	if (!file) throw std::runtime_error("Cannot open file");

	for (const auto& chunk : chunks) 
	{
		file.write(chunk.chunk.data(), chunk.chunkSize);
	}
	file.close();
}
