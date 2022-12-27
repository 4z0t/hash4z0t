
#include <string.h>
#include "../NoiseProtection/hamming.h"
#include <bitset>



using std::cout;
using std::endl;

bool TestHAM(const  BitsAndBytes::BytesVector& original)
{
	size_t len = original.size();
	auto bits = NoiseProtection::Hamming::ToHamming(NoiseProtection::ToBits(original));
	std::cout << original.size() << "\n";
	std::cout << bits.size() << "\n";
	try
	{

		auto bytes = NoiseProtection::ToBytes(NoiseProtection::Hamming::FromHamming(bits, len * 8));
		if (bytes == original)
			return true;
		else
		{
			std::cout << std::string(bytes.begin(), bytes.end()) << "\n";
			return false;
		}
	}
	catch (NoiseProtection::Hamming::FlipBitException)
	{
		std::cout << "caused bit flip\n";
		return false;
	}
}

bool TestHAM(const std::string& s)
{
	auto original = NoiseProtection::BytesVector(s.begin(), s.end());
	return TestHAM(original);
}


std::vector<std::string> testCases = {
	"A",
	"AA",
	"AAA",
	"AAAA",
	"AAAAA",
	"AAAAAA",
	"AAAAAAAAAAAAAAAAAAAAAAAAAAA",
	"abababbabababbababababb",
	"asfdsdsfiutydsaif gehsafj gds fhds fkvgdv f",
	"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB",
	"\0",
	"1234567890",
	"F))F))))F))F)F)F))F)F)F)F)F))F))"
	"AB",
	"ABC",
	"AABB",
	"AABBC",
	"AABBCC",


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

	/*
	std::string s = "A";
	auto bits = NoiseProtection::ToBits(NoiseProtection::BytesVector(s.begin(), s.end()));
	cout << bits << endl;
	bits = NoiseProtection::Hamming::EncodeMessage(bits);
	cout << bits << endl;
	try
	{
		bits = NoiseProtection::Hamming::DecodeBlock(bits);
		cout << bits << endl;

	}
	catch (NoiseProtection::Hamming::FlipBitException)
	{
		cout << NoiseProtection::Hamming::FindFlip(bits) << endl;
	}
	return 0;
	*/
	

	for (auto& s : testCases)
	{
		if (!TestHAM(s))
		{
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
		if (!TestHAM(RandomBytes(1'000'000)))
		{
			std::cerr << "Failed  compression for \t\t\"" << i << "\"\n";
		}
		else
		{
			std::cout << "Success compression for \t\"" << i << "\"\n";
		}
	}

}