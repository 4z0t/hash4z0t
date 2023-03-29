



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


uint32_t EncodeByteInQuad(uint32_t quad, uint8_t byte)
{
	union
	{
		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t alpha;
		};
		uint32_t quad = quad;
	}pixel;

	pixel.blue &= 0xFC;
	pixel.blue |= (byte >> 6) & 0x3;
	pixel.green &= 0xFC;
	pixel.green |= (byte >> 4) & 0x3;
	pixel.red &= 0xFC;
	pixel.red |= (byte >> 2) & 0x3;
	pixel.alpha &= 0xFC;
	pixel.alpha |= (byte) & 0x3;
	return pixel.quad;
}


void encoder(Path input_path, Path bmp_path, Path output_path)
{
	using namespace BitsAndBytes;


	File bmp_file(bmp_path);
	if (!bmp_file.Open(true))
	{
		cout << "Couldn't open file " << bmp_path << endl;
		return;
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



	uint32_t offset = 0;

	for (size_t i = 0; i < 10; )
	{
		output_file.Put(bmp_file.Get());
	}

	offset = bmp_file.Read<uint32_t>();
	output_file.Write(offset);
	std::cout << "Offset: " << offset << std::endl;

	for (size_t i = 14; i < offset; i++)
	{
		output_file.Put(bmp_file.Get());
	}

	uint32_t bmp_file_size = bmp_file.GetSize();
	for (size_t i = offset; i < bmp_file_size; i += 4)
	{
		output_file.Write(EncodeByteInQuad(bmp_file.Read<uint32_t>(), input_file.Get()));
	}
	std::cout << "successfully encoded file!" << std::endl;

}

void decoder(Path input_path, Path bmp_path)
{
	using namespace BitsAndBytes;

	File key_file(bmp_path);
	if (!key_file.Open(true))
	{
		cout << "Couldn't open file " << bmp_path << endl;
		return;
	}






	File input_file(input_path);
	if (!input_file.Open(true))
	{
		cout << "Couldn't open file " << input_path << endl;
		return;
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

		decoder(input_path, bmp_path);
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