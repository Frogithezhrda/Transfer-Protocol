#pragma once
#include <vector>
#include <fstream>
#include "FileException.hpp"

constexpr const int BYTE_COUNT = 16000;

struct Chunk
{
	std::vector<char> chunk;
	int chunkSize;
};

class FileChunk
{
public:
	Chunk serialize(std::ifstream& file) const;
	void deserialize(const std::vector<char>& chunkBytes);
	void writeChunksToFile(const std::string& fileName);
	std::vector<Chunk>& getFileChunks();
private:
	std::vector<Chunk> chunks;
};

