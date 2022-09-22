
#include <iostream>
#include <fstream>
#include "h4z0t.h"
#include "FileRegister.h"
#include "File.h"


#define FLAG_PREFIX "--"

#define ENCODE_ "encode"
#define DECODE_ "decode"

#define DECODE_FLAG (FLAG_PREFIX DECODE_)
#define ENCODE_FLAG (FLAG_PREFIX ENCODE_)

#define is_decode(s) (strcmp((s), DECODE_FLAG) == 0)
#define is_encode(s) (strcmp((s), ENCODE_FLAG) == 0)

void dir_test()
{

}


int main(int argc, char* argv[])
{
	if (argc < 3)
	{
#if _DEBUG 
		std::cout << "Encoding: \n";
		Encoder encoder;
		encoder.MakeHeader();
		auto s = "C:\\Users\\gaste\\Downloads\\";
		auto files = FileRegister::GetFileList(s);
		for (auto& f : files)
		{

			std::cout << f.path() << std::endl;
			std::cout << f.path().lexically_relative(s) << std::endl;
			File file(f.path(), f.path().lexically_relative(s));
			encoder.WriteFileHeader(file.MakeHeader());
		}
#elif
		std::cout << "Not enough arguments";
#endif
	}
	else
	{
		if (is_decode(argv[1]))
		{
			std::cout << "Decoding: \n";
			for (int i = 2; i < argc; i++)
			{
				std::cout << "\t" << argv[i] << "\n";
			}
			Decoder decoder{ std::string(argv[2]) };
			if (!decoder.VerifyFormat())
			{
				std::cout << "File is not an encoded one" << std::endl;
				return 0;
			}
			if (!decoder.VerifyVersion())
			{
				std::cout << "File has incorrect version of format" << std::endl;
				return 0;
			}
			u32 n = decoder.Read<u32>();
			for (u32 i = 0; i < n; i++)
			{
				File::Header h = decoder.Read<File::Header>();
				std::cout << "data " << h.dataLen << std::endl;
				std::cout << "name " << h.nameLen << std::endl;
				std::string name = decoder.ReadString(h.nameLen);
				std::cout << name << std::endl;

			}


		}
		else if (is_encode(argv[1]))
		{
			std::cout << "Encoding: \n";
			for (int i = 2; i < argc; i++)
			{
				std::cout << "\t" << argv[i] << "\n";
			}
			Encoder encoder;

			auto files = FileRegister::GetFileList(argv[2]);
			encoder.MakeHeader(files.size());
			for (auto& f : files)
			{

				std::cout << f.path() << std::endl;
				std::cout << f.path().lexically_relative(argv[2]) << std::endl;
				File file(f.path(), f.path().lexically_relative(argv[2]));
				File::Header h = file.MakeHeader();
				encoder.WriteFileHeader(h);
				encoder.WriteString(file.GetName().u8string());

			}


		}
		else
		{
			std::cout << "Unknown flag. Available flags are:\n" << DECODE_FLAG << "\n" << ENCODE_FLAG;
		}
	}
	return 0;
}

