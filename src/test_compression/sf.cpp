
#include <string.h>
#include "../Compression/shannon_fano.h"
#include <bitset>

int main(int argc, char* argv[])
{

	char buff[256]{};
	strcpy(buff, "aabbabcaaaaa");


	auto bits = Compression::ShannonFano::Compress(Compression::CStringToData(buff));
	auto bytes = Compression::ToBytes(bits);
	for (auto b : bytes)
	{
		std::cout << std::bitset<8>(b) << std::endl;
	}
	auto bits2 = Compression::ToBits(bytes);
	for (size_t i = 0; i < bits2.size(); i++)
	{
		if (i % 8 == 0)
		{
			std::cout << std::endl;
		}
		std::cout << bits2[i];
	}
	std::cout << std::endl;
	auto bytes2 = Compression::ShannonFano::Decompress(bits2);
	for (auto b : bytes2)
	{
		std::cout << b ;
	}

}