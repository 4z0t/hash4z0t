#include "../Encryption/rsa.h"


bool TestRSA(const BitsAndBytes::BytesVector& original, size_t p, size_t q)
{
	size_t n = p * q;
	size_t t = Encryption::EulersTotient(p, q);

	size_t e = Encryption::CalcE(t);
	size_t d = Encryption::CalcD(e, t);


	std::cout << "e:\t" << e << std::endl << "d:\t" << d << std::endl;

	auto v = Encryption::RSA::Encode(original, e, n);
	auto result = Encryption::RSA::Decode(v, d, n);

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
	using namespace Encryption::RSA;
	std::srand(0);

	size_t p = MakePrime(100000);
	size_t q = MakePrime(100000);


	std::cout << p << std::endl << q << std::endl;

	for (size_t i = 0; i < 10; i++)
	{
		if (!TestRSA(RandomBytes(1'000), p, q))
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