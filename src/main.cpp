
#include <iostream>
#include <fstream>
#include "h4z0t.h"


#define FLAG_PREFIX "--"

#define ENCODE_ "encode"
#define DECODE_ "decode"

#define DECODE_FLAG (FLAG_PREFIX DECODE_)
#define ENCODE_FLAG (FLAG_PREFIX ENCODE_)

#define is_decode(s) (strcmp((s), DECODE_FLAG) == 0)
#define is_encode(s) (strcmp((s), ENCODE_FLAG) == 0)

using namespace H4z0t;

void dir_test()
{


}


int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Not enough arguments";
		return 1;
	}

	if (is_decode(argv[1]))
	{
		std::string savePath;
		std::cout << "Decoding: \n";
		for (int i = 2; i < argc; i++)
		{
			if (strcmp("--into", argv[i]) == 0)
			{
				if (i + 1 == argc)

					std::cout << "incorrect location name" << std::endl;
				else
					savePath = argv[++i];

			}
			else
				std::cout << "\t" << argv[i] << "\n";
		}
		try
		{
			Decoder decoder{ std::string(argv[2]) };
			decoder.Start(savePath);
		}
		catch (const H4z0t::CantOpenFileException& e)
		{
			std::cerr << "Unable to open file " << e.what();
			return 1;
		}
		catch (const H4z0t::InvaildFileException& e)
		{
			std::cerr << "Given file is invilid";
			return 1;
		}


	}
	else if (is_encode(argv[1]))
	{
		std::cout << "Encoding: \n";
		for (int i = 2; i < argc; i++)
		{
			std::cout << "\t" << argv[i] << "\n";
		}

		try
		{

			Encoder encoder;

			encoder.Start(argv[2]);
		}
		catch (std::exception)
		{
			return 1;
		}



	}
	else
	{
		std::cout << "Unknown flag. Available flags are:\n" << DECODE_FLAG << "\n" << ENCODE_FLAG;
		return 1;
	}

	return EXIT_SUCCESS;
}

