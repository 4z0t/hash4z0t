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
			try
			{
				ProcessFile(file);
			}
			catch (CantOpenFileException& e)
			{
				std::cerr << "Unable to open file " << f.path() << std::endl;
			}


		}

	}

	void Encoder::ProcessFile(File& file)
	{

		if (!file.Open(true))throw CantOpenFileException();
		File::Header h = file.MakeHeader();
		h.comp = compression;
		h.enc = encryption;
		h.prot = protection;
		WriteFileHeader(h);
		_outputFile.Write(file.GetName().u8string());

		if (compression == CompressionType::None)
		{
			std::cout << "Encoding with plain code" << std::endl;
			for (uintmax_t i = 0; i < h.dataLen; i++)
			{
				_outputFile.Put(file.Get());
			}
			return;
		}


		if (compression == CompressionType::LZ77)
		{
			/*FileData file_data;
			file_data.Collect(file.GetPath());
			const auto data = file_data.GetData();*/
			std::cout <<"Encoding with LZ77" << std::endl;
			Compression::BytesVector cache;
			cache.reserve(3);

			Compression::LZ77::SlidingWindow window;
			//window.SetRefUnit(file_data.GetLessCharInFile());


			for (uintmax_t i = 0; i < h.dataLen; i++)
			{
				Compression::unit u = file.Get();
				if (window.Encode(u, cache))
				{
					_outputFile.Write(cache);
					cache.clear();
				}
			}
			auto end = window.End();
			_outputFile.Write(end);
			return;
		}

		if (compression == CompressionType::SF)
		{
			std::cout << "Encoding with Shannon Fano" << std::endl;
			/*Compression::BytesVector cache;
			cache.reserve(3);

			Compression::LZ77::SlidingWindow window;
			window.SetRefUnit(file_data.GetLessCharInFile());


			for (uintmax_t i = 0; i < h.dataLen; i++)
			{
				Compression::unit u = file.Get();
				if (window.Encode(u, cache))
				{
					_outputFile.Write(cache);
					cache.clear();
				}
				auto end = window.End();
				_outputFile.Write(end);
			}*/
			return;
		}

	}




	void Encoder::MakeHeader(u32 fileCount)
	{
		this->MakeHeader();
		_outputFile.Write(fileCount);
	}


	Encoder::~Encoder() {}
}