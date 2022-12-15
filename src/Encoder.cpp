#include "Encoder.h"
#include "Encoder.h"
#include "Encoder.h"
namespace H4z0t {

	Encoder::Encoder()
	{
		_OpenFile(Path(DEFAULT_OUTPUT_PATH));
	}

	Encoder::Encoder(const String& s)
	{
		_OpenFile(s);
	}

	Encoder::Encoder(const Path& path)
	{
		_OpenFile(path);
	}

	Encoder::Encoder(const DirEntry& dir_entry)
	{
		_OpenFile(dir_entry);
	}

	Encoder::Encoder(const Arguments& args)
	{
		_OpenFile(args.savePath);

		compression = args.compression;
		protection = args.protection;
		encryption = args.encryption;
	}

	void Encoder::_OpenFile(const Path& path)
	{
		if (!_outputFile.Open(false, path))
			throw CantOpenFileException(path.u8string().c_str());
	}

	void Encoder::MakeHeader()
	{
		_outputFile.Write(FMT_HEADER_, strlen(FMT_HEADER_));
		_outputFile.Write(FMT_VERSION);
	}


	void Encoder::WriteFileHeader(File::Header h)
	{
		return _outputFile.Write(h);
	}


	void Encoder::Start(const Path& filesPath)
	{
		auto files = GetFileList(filesPath);
		MakeHeader(files.size());
		std::cout << files.size() << std::endl;
		for (auto& f : files)
		{

			std::cout << f.path() << std::endl;
			std::cout << f.path().lexically_relative(filesPath) << std::endl;
			File file(f.path(), f.path().lexically_relative(filesPath));
			if (file.Open(true))
			{

				File::Header h = file.MakeHeader();
				WriteFileHeader(h);
				_outputFile.Write(file.GetName().u8string());
				for (uintmax_t i = 0; i < h.dataLen; i++)
				{
					_outputFile.Put(file.Get());
				}

			}
			else
			{
				std::cerr << "Unable to open file " << f.path() << std::endl;
			}

		}

	}






	void Encoder::MakeHeader(u32 fileCount)
	{
		this->MakeHeader();
		_outputFile.Write(fileCount);
	}


	Encoder::~Encoder() {}
}