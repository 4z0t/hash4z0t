
#include <iostream>
#include <fstream>
#include "h4z0t.h"


#define ARG_PREFIX "--"

#define ENCODE_ "encode"
#define DECODE_ "decode"

#define DECODE_FLAG (ARG_PREFIX DECODE_)
#define ENCODE_FLAG (ARG_PREFIX ENCODE_)

#define is_decode(s) (strcmp((s), DECODE_FLAG) == 0)
#define is_encode(s) (strcmp((s), ENCODE_FLAG) == 0)

using namespace H4z0t;

using std::cout;
using std::endl;

void dir_test()
{


}


int main(int argc, char** argv)
{
	Arguments args = ArgumentsManager::Process(argc, argv);

	if (args.mode == Mode::IncorrectArguments)
	{
		cout << "Incorrect arguments" << endl;
		return 1;
	}

	if (args.mode == Mode::NotEnoughArguments)
	{
		cout << "Not enough arguments" << endl;
		return 1;
	}

	if (args.mode == Mode::Decode)
	{
		for (const auto& path : args.targets)
		{

			try
			{
				Decoder decoder{ path };
				decoder.Start(args.savePath);
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
		return EXIT_SUCCESS;
	}
	if (args.mode == Mode::Encode)
	{
		try
		{
			Encoder encoder(args.savePath);
			encoder.Start(args.targets[0]);
		}
		catch (std::exception)
		{
			return 1;
		}
		return EXIT_SUCCESS;
	}
}

