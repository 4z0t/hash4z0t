#include "../Encryption/rsa.h"


bool TestRSA(const BitsAndBytes::BytesVector& original, size_t p, size_t q)
{
	size_t n = p * q;
	size_t t = Encryption::EulersTotient(p, q);

	size_t e = Encryption::CalcE(t);
	size_t d = Encryption::CalcD(e, t);

	auto v = Encryption::RSA::Encode(original, e, n);
	auto result = Encryption::RSA::Decode(v, d, n);


	return result == original;

}

bool TestUIntToBytesAndBack(const BitsAndBytes::BytesVector& original)
{
	auto v = Encryption::BytesToUInts(original);
	auto result = Encryption::UIntsToBytes(v);
	return result == original;
}

BitsAndBytes::BytesVector RandomBytes(size_t size)
{
	using namespace BitsAndBytes;
	BytesVector res(size);
	for (size_t i = 0; i < size; i++)
		res[i] = std::rand();
	return res;
}



size_t MakePrime(size_t max_v)
{
	size_t v = std::rand() % max_v;
	while (!Encryption::IsPrime(v))
	{
		v++;
	}
	return v;
}


int main()
{
	
	std::srand(0);

	for (size_t i = 0; i < 10; i++)
	{
		if (!TestUIntToBytesAndBack(RandomBytes(1'000)))
		{
			std::cerr << "Failed  TestUIntToBytesAndBack for \t\t\"" << i << "\"\n";
		}
		else
		{
			std::cout << "Success TestUIntToBytesAndBack for \t\"" << i << "\"\n";
		}
	}

	//std::cout << Encryption::PowMod(3, 8, 3) << std::endl;


	size_t p = MakePrime(UINT64_MAX);
	size_t q = MakePrime(UINT64_MAX);
	p = 3557;
	q = 2579;


	std::cout << p << std::endl << q << std::endl << p * q << std::endl;

	for (size_t i = 0; i < 10; i++)
	{
		if (!TestRSA(RandomBytes(16), p, q))
		{
			std::cerr << "Failed  encryption for \t\t\"" << i << "\"\n";
		}
		else
		{
			std::cout << "Success encryption for \t\"" << i << "\"\n";
		}
	}
	return 0;
}