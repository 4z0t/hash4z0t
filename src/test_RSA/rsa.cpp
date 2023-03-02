#include "../Encryption/rsa.h"
#include "../FileData.h"

using namespace std;
using namespace H4z0t;

bool TestRSA(const BitsAndBytes::BytesVector& original, size_t p, size_t q)
{
	size_t n = p * q;
	size_t t = Encryption::EulersTotient(p, q);

	size_t e = Encryption::CalcE(t);
	size_t d = Encryption::CalcD(e, t);

	std::cout << e << std::endl << d << std::endl;

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



void TestRSA()
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


	size_t p = MakePrime(UINT64_MAX);
	size_t q = MakePrime(UINT64_MAX);


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
}








void encoder(Path input_path, Path key_path, Path output_path)
{
	using namespace BitsAndBytes;
	size_t e;
	size_t n;
	{
		File key_file(key_path);
		if (!key_file.Open(true))
		{
			cout << "Couldn't open file " << key_path << endl;
			return;
		}

		if (key_file.GetSize() != sizeof(size_t) * 2)
		{
			cout << "Key file has wrong size" << endl;
			return;
		}

		e = key_file.Read<size_t>();
		n = key_file.Read<size_t>();
	}



	File input_file(input_path);
	if (!input_file.Open(true))
	{
		cout << "Couldn't open file " << input_path << endl;
		return;
	}

	File output_file(output_path);
	if (!output_file.Open(false))
	{
		cout << "Couldn't open file " << output_path << endl;
		return;
	}



	for (size_t i = 0; i < input_file.GetSize(); i++)
	{
		size_t v = Encryption::RSA::Encode(input_file.Get(), e, n);
		output_file.Write(v);
	}
}

void decoder(Path input_path, Path key_path, Path output_path)
{
	using namespace BitsAndBytes;
	size_t d;
	size_t n;
	{
		File key_file(key_path);
		if (!key_file.Open(true))
		{
			cout << "Couldn't open file " << key_path << endl;
			return;
		}

		if (key_file.GetSize() != sizeof(size_t) * 2)
		{
			cout << "Key file has wrong size" << endl;
			return;
		}

		d = key_file.Read<size_t>();
		n = key_file.Read<size_t>();
	}



	File input_file(input_path);
	if (!input_file.Open(true))
	{
		cout << "Couldn't open file " << input_path << endl;
		return;
	}

	File output_file(output_path);
	if (!output_file.Open(false))
	{
		cout << "Couldn't open file " << output_path << endl;
		return;
	}



	for (size_t i = 0; i < input_file.GetSize() / sizeof(size_t); i++)
	{
		size_t v = Encryption::RSA::Encode(input_file.Read<size_t>(), d, n);
		output_file.Put(v);
	}
}


void GenerateKey()
{

	std::srand(std::time(0));

	size_t p = MakePrime(UINT64_MAX);
	size_t q = MakePrime(UINT64_MAX);

	std::cout << p << std::endl << q  << std::endl;

	size_t n = p * q;
	size_t t = Encryption::EulersTotient(p, q);

	size_t e = Encryption::CalcE(t);
	size_t d = Encryption::CalcD(e, t);

	Path open_key_path = "open.key";
	File open_key_file(open_key_path);
	if (!open_key_file.Open(false))
	{
		cout << "Couldn't open file " << open_key_path << endl;
		return;
	}

	Path secret_key_path = "secret.key";
	File secret_key_file(secret_key_path);
	if (!secret_key_file.Open(false))
	{
		cout << "Couldn't open file " << secret_key_path << endl;
		return;
	}

	secret_key_file.Write(d);
	secret_key_file.Write(n);

	open_key_file.Write(e);
	open_key_file.Write(n);


	cout << "generated keys 'open.key' and 'secret.key'" << endl;
}


int main(int argc, char** argv)
{

	if (argc == 1)
	{
		cout << "Generating Keys:" << endl;
		GenerateKey();
		return 0;
	}
	if (argc < 5) {
		cout << "Wrong arguments!" << endl;
		return 0;
	}

	Path input_path = argv[1];
	Path key_path = argv[2];
	Path output_path = argv[3];
	string flag = argv[4];



	if (flag == "-decode")
	{

		decoder(input_path, key_path, output_path);
	}
	else if (flag == "-encode")
	{
		encoder(input_path, key_path, output_path);

	}
	else
	{
		cout << "unkown flag " << flag;
	}
	return 0;
}