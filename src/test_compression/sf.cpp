
#include <string.h>
#include "../Compression/shannon_fano.h"
#include <bitset>


using std::cout;
using std::endl;

bool TestSF(const std::string& s)
{
	auto original = Compression::BytesVector(s.begin(), s.end());
	auto bits = Compression::ToBytes(Compression::ShannonFano::Compress(original));
	std::cout << original.size() << "\n";
	std::cout << bits.size() << "\n";
	auto bytes = Compression::ShannonFano::Decompress(bits);
	if (bytes == original)
		return true;
	else
	{
		std::cout << std::string(bytes.begin(), bytes.end()) << "\n";
		return false;
	}
}

bool TestSF(const  BitsAndBytes::BytesVector& original)
{
	auto bits = Compression::ToBytes(Compression::ShannonFano::Compress(original));
	std::cout << original.size() << "\n";
	std::cout << bits.size() << "\n";
	auto bytes = Compression::ShannonFano::Decompress(bits);
	if (bytes == original)
		return true;
	else
	{
		cout << "Original len: " << original.size() << endl
			<< "Decoded len: " << bytes.size() << endl;
		return false;
	}
}



std::vector<std::string> testCases = {
	"AAAAAAAAAAAAAAAAAAAAAAAAAAA",
	"abababbabababbababababb",
	"asfdsdsfiutydsaif gehsafj gds fhds fkvgdv f",
	"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB",
	"",
	"\0",
	"1234567890",
	"F))F))))F))F)F)F))F)F)F)F)F))F))"
	"AB",
	"ABC",
	"AABB",
	"AABBC",
	"AABBCC",
	"",
	"",
	"",
	"",
	"",


};

BitsAndBytes::BytesVector RandomBytes(size_t size)
{
	using namespace BitsAndBytes;
	BytesVector res(size);
	for (size_t i = 0; i < size; i++)
		res[i] = std::rand();
	return res;
}


int main(int argc, char* argv[])
{
	for (auto& s : testCases)
	{
		if (!TestSF(s))
		{
			std::cerr << s << std::endl;
			std::cerr << "Failed  compression for \t\t\"" << s << "\"\n";
		}
		else
		{
			std::cout << "Success compression for \t\"" << s << "\"\n";
		}
	}
	std::srand(0);
	for (size_t i = 0; i < 10; i++)
	{
		if (!TestSF(RandomBytes(5000)))
		{
			std::cerr << "Failed  compression for \t\t\"" << i << "\"\n";
		}
		else
		{
			std::cout << "Success compression for \t\"" << i << "\"\n";
		}
	}


}