

#include "internalhash4z0t.h"


#define ARG_PREFIX "--"

#define ENCODE_ "encode"
#define DECODE_ "decode"
#define SAVEPATH_ "into"

#define DECODE_FLAG (ARG_PREFIX DECODE_)
#define ENCODE_FLAG (ARG_PREFIX ENCODE_)
#define SAVEPATH_FLAG (ARG_PREFIX SAVEPATH_)


#define is_decode(s) (strcmp((s), DECODE_FLAG) == 0)
#define is_encode(s) (strcmp((s), ENCODE_FLAG) == 0)
#define is_into(s) (strcmp((s), SAVEPATH_FLAG) == 0)

#define FLAG_PREFIX "-"

#define LZ77_FLAG (FLAG_PREFIX "lz77")
#define SHANNON_FANO_FLAG (FLAG_PREFIX "shannon-fano")
#define HAMMING_FLAG (FLAG_PREFIX "hamming")

#define is_lz77(s) (strcmp((s), LZ77_FLAG) == 0)
#define is_sf(s) (strcmp((s), SHANNON_FANO_FLAG) == 0)
#define is_hamming(s) (strcmp((s), HAMMING_FLAG) == 0)

namespace H4z0t
{
	class ArgumentsManager
	{
	public:
		ArgumentsManager() = delete;

		static Arguments Process(int argc, char** argv)
		{
			Arguments args;

			if (argc < 3)
			{
				args.mode = Mode::NotEnoughArguments;
				return args;
			}

			Mode mode = Mode::None;
			for (int i = 1; i < argc; i++)
			{
				const char* cur = argv[i];

				//decode flag
				if (is_decode(cur))
				{
					//we already have a mode
					if (mode != Mode::None)
					{
						args.mode = Mode::IncorrectArguments;
						break;
					}
					args.mode = Mode::Decode;
					mode = Mode::Decode;
					continue;
				}

				//encode flag
				if (is_encode(cur))
				{
					//we already have a mode
					if (mode != Mode::None)
					{
						args.mode = Mode::IncorrectArguments;
						break;
					}
					args.mode = Mode::Encode;
					mode = Mode::Encode;
					continue;
				}

				//into flag
				if (is_into(cur))
				{
					//into flag again or no path as next arg
					if (!args.savePath.empty() || i + 1 == argc)
					{
						args.mode = Mode::IncorrectArguments;
						break;
					}
					i++;
					args.savePath = String(argv[i]);
					continue;
				}


				if (is_lz77(cur))
				{
					if (args.compression != CompressionType::None)
					{
						args.mode = Mode::IncorrectArguments;
						break;
					}
					args.compression = CompressionType::LZ77;
					continue;
				}

				if (is_sf(cur))
				{
					if (args.compression != CompressionType::None)
					{
						args.mode = Mode::IncorrectArguments;
						break;
					}
					args.compression = CompressionType::SF;
					continue;
				}

				if (is_hamming(cur))
				{
					if (args.protection != ProtectionType::None)
					{
						args.mode = Mode::IncorrectArguments;
						break;
					}
					args.protection = ProtectionType::Hamming;
					continue;
				}

				//adding targets
				if (mode == Mode::Decode || mode == Mode::Encode)
				{
					args.targets.push_back(Path(cur));
					continue;
				}
				else //no modes and we have something else
				{
					args.mode = Mode::IncorrectArguments;
					break;
				}
			}


			CheckArgs(args);
			return args;
		}

	private:
		static void CheckArgs(Arguments& args)
		{
			if (args.savePath.empty())args.savePath = Path(DEFAULT_OUTPUT_PATH);
			if (args.targets.empty())args.mode = Mode::IncorrectArguments;
		}
	};
}