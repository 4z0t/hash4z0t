

#include <string>
#include "../BitsAndBytes/BitsAndBytes.h"
#include "../FileData.h"

using namespace std;

using namespace H4z0t;
using namespace BitsAndBytes;

BytesVector ApplyKey(const BytesVector& input, const BytesVector& key)
{
	BytesVector output(input.size());
	for (size_t i = 0; i < input.size(); i++)
	{
		output[key[i]] = input[i];
	}
	return output;
}

BytesVector InverseKey(const BytesVector& input, const BytesVector& key)
{
	BytesVector output(input.size());
	for (size_t i = 0; i < input.size(); i++)
	{
		output[i] = input[key[i]];
	}
	return output;
}

void encoder(Path input_path, Path key_path, Path output_path)
{
	BytesVector key;
	{
		File key_file(key_path);
		if (!key_file.Open(true))
		{
			cout << "Couldn't open file " << key_path << endl;
			return;
		}


		for (size_t i = 0; i < key_file.GetSize(); i++)
		{
			key.push_back(key_file.Get() - '1');
		}
	}
	cout << "Key size is " << key.size() << endl;


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

	BytesVector buffer;
	buffer.reserve(key.size());

	size_t n = input_file.GetSize() / key.size();
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < key.size(); j++)
		{
			buffer.push_back(input_file.Get());
		}
		output_file.Write(ApplyKey(buffer, key));
		buffer.clear();
	}
	for (size_t i = 0; i < input_file.GetSize() - n * key.size(); i++)
	{
		output_file.Put(input_file.Get());
	}
}

void decoder(Path input_path, Path key_path, Path output_path)
{
	BytesVector key;
	{
		File key_file(key_path);
		if (!key_file.Open(true))
		{
			cout << "Couldn't open file " << key_path << endl;
			return;
		}


		for (size_t i = 0; i < key_file.GetSize(); i++)
		{
			key.push_back(key_file.Get() - '1');
		}
	}
	cout << "Key size is " << key.size() << endl;


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

	BytesVector buffer;
	buffer.reserve(key.size());

	size_t n = input_file.GetSize() / key.size();
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < key.size(); j++)
		{
			buffer.push_back(input_file.Get());
		}
		output_file.Write(InverseKey(buffer, key));
		buffer.clear();
	}
	for (size_t i = 0; i < input_file.GetSize() - n * key.size(); i++)
	{
		output_file.Put(input_file.Get());
	}
}


int main(int argc, char** argv)
{
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