
#include <string.h>
#include "../Compression/lz77.h"
#include <bitset>


bool TestLZ(const std::string& s)
{
	auto original = Compression::BytesVector(s.begin(), s.end());
	auto bits = Compression::LZ77::Compress(original);

	auto bytes = Compression::LZ77::Decompress(bits);
	if (bytes == original)
		return true;
	else
	{
		std::cout << std::string(bytes.begin(), bytes.end()) << "\n";
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
	"ababababababababcabc",
	"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAB",
	"",
	"",
	"112123123412345",
	"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",


};


int main(int argc, char* argv[])
{
	for (auto& s : testCases)
	{
		if (!TestLZ(s))
		{
			std::cerr << "Failed  compression for \t\t\"" << s << "\"\n";
		}
		else
		{
			std::cout << "Success compression for \t\"" << s << "\"\n";
		}
	}
	/*std::string s = "00000";
	auto original = Compression::BytesVector(s.begin(), s.end());
	auto bits = Compression::LZ77::Compress(original);
	for (auto u : bits)
	{
		std::cout << (int)u << std::endl;
	}*/

}