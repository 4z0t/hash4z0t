



#include "../Encryption/bmp2bit.h"
#include "../FileData.h"

using namespace std;
using namespace H4z0t;




BitsAndBytes::BytesVector RandomBytes(size_t size)
{
	using namespace BitsAndBytes;
	BytesVector res(size);
	for (size_t i = 0; i < size; i++)
		res[i] = std::rand();
	return res;
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
		
		output_file.Put(v);
	}
}


int main(int argc, char** argv)
{
	if (argc < 5) {
		cout << "Wrong arguments!" << endl;
		return 0;
	}

	Path input_path = argv[1];
	Path bmp_path = argv[2];
	Path output_path = argv[3];
	string flag = argv[4];



	if (flag == "-decode")
	{

		decoder(input_path, bmp_path, output_path);
	}
	else if (flag == "-encode")
	{
		encoder(input_path, bmp_path, output_path);

	}
	else
	{
		cout << "unkown flag " << flag;
	}
	return 0;
}