
#include <string.h>
#include "../Compression/shannon_fano.h"
#include <bitset>


bool TestSF(const std::string& s)
{
	auto original = Compression::BytesVector(s.begin(), s.end());
	auto bits = Compression::ShannonFano::Compress(original);

	auto bytes = Compression::ShannonFano::Decompress(bits);
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
	"",
	"",
	"",
	"",
	"",
	 

};


int main(int argc, char* argv[])
{
	for (auto& s : testCases)
	{
		if (!TestSF(s))
		{
			std::cerr << "Failed  compression for \t\t\"" << s << "\"\n";
		}
		else
		{
			std::cout << "Success compression for \t\"" << s << "\"\n";
		}
	}


}