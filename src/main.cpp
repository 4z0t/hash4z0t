
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


int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Not enough arguments";

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
		}
		else if (is_encode(argv[1]))
		{
			std::cout << "Encoding: \n";
			for (int i = 2; i < argc; i++)
			{
				std::cout << "\t" << argv[i] << "\n";
			}
			Encoder encoder;
			encoder.MakeHeader();

		}
		else
		{
			std::cout << "Unknown flag. Available flags are:\n" << DECODE_FLAG << "\n" << ENCODE_FLAG;
		}
	}
	return 0;
}

