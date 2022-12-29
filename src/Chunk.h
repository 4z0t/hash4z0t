#include <cstddef>

class Chunk
{
public:
	static const std::size_t MAX_SIZE = 1024 * 256;

	Chunk();
	~Chunk();



private:
	std::size_t size;
	std::byte* _data;
};

Chunk::Chunk()
{
}

Chunk::~Chunk()
{
	if (_data != nullptr)delete[]_data;
}