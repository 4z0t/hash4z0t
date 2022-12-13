
#include <string.h>
#include "../NoiseProtection/hamming.h"
#include <bitset>



using std::cout;
using std::endl;



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


int main(int argc, char* argv[])
{
	cout << Protection::Pow2(4) << endl;
	cout << Protection::Hamming::CalculateCheckBitsCount(255) << endl;
	std::string s = "a";
	auto bits = Protection::ToBits(Protection::BytesVector(s.begin(), s.end()));
	cout << bits << endl;
	bits = Protection::Hamming::EncodeMessage(bits);
	cout << bits << endl;
	size_t pos = bits.size() - 1;
	bits[pos] = !bits[pos];
	try
	{
		bits = Protection::Hamming::DecodeBlock(bits);
		cout << bits << endl;

	}
	catch (Protection::Hamming::FlipBitException)
	{
		cout << Protection::Hamming::FindFlip(bits) << endl;
	}
}