#pragma once
#include "internalhash4z0t.h"
#include "File.h"
#include "FileData.h"

namespace H4z0t
{

	class Decoder
	{
	public:



		Decoder();
		Decoder(String);
		Decoder(Path);
		Decoder(DirEntry);
		Decoder(const Path& path, const Arguments& args);


		bool VerifyFile()
		{
			return VerifyFormat() && VerifyVersion();
		}

		void Start(const Path& savePath);
		void ProcessFile(File&, const File::Header&);


		~Decoder();
	protected:

		u32 FilesCount()
		{
			return _inputFile.Read<u32>();
		}
		bool VerifyFormat();
		bool VerifyVersion();
		void _OpenFile(Path);


		Path _filePath;
		File _inputFile;
		CompressionType compression = CompressionType::None;
		ProtectionType protection = ProtectionType::None;
		EncryptionType encryption = EncryptionType::None;

	};
}


#include "Compression/lz77.h"
#include "Compression/shannon_fano.h"
namespace H4z0t {
	Decoder::Decoder()
	{
		_OpenFile(std::filesystem::current_path());
	}

	Decoder::Decoder(String s)
	{
		_OpenFile(Path(s));
	}

	Decoder::Decoder(Path path)
	{
		_OpenFile(path);
	}

	Decoder::Decoder(DirEntry entry)
	{
		_OpenFile(entry);
	}

	Decoder::Decoder(const Path& path, const Arguments& args)
	{
		_OpenFile(path);
		compression = args.compression;
		protection = args.protection;
		encryption = args.encryption;
	}


	void Decoder::Start(const Path& savePath)
	{

		if (!VerifyFile())
		{
			throw InvaildFileException();
		}

		Path curDir = FS::current_path();
		if (!savePath.empty())
		{
			curDir /= savePath;
		}
		u32 filesCount = FilesCount();


		for (u32 i = 0; i < filesCount; i++)
		{
			File::Header h = _inputFile.Read<File::Header>();
			std::string name = _inputFile.ReadString(h.nameLen);
			File file(curDir / name);
			FS::create_directories((curDir / name).parent_path());
			try
			{
				ProcessFile(file, h);
			}
			catch (CantOpenFileException)
			{
				std::cout << "File not opened " << name << std::endl;
			}
		}



	}


	void  Decoder::ProcessFile(File& file, const File::Header& header)
	{
		if (!file.Open(false))throw CantOpenFileException();

		std::cout << "File opened\t" << file.GetPath() << std::endl;

		if (header.comp == CompressionType::None)
		{
			for (uintmax_t i = 0; i < header.dataLen; i++)
			{
				file.Put(_inputFile.Get());
			}
			return;
		}


		if (header.comp == CompressionType::LZ77)
		{
			Compression::BytesVector cache;
			Compression::LZ77::SlidingWindow window;

			window.SetRefUnit(header.data[0]);

			for (uintmax_t i = 0; i < header.dataLen;)
			{
				Compression::unit u = _inputFile.Get();
				if (window.Decode(u, cache))
				{
					i += cache.size();
					file.Write(cache);
					cache.clear();
				}
			}
			return;
		}

		if (header.comp == CompressionType::SF)
		{
			Compression::ShannonFano::Decoder d;
			Compression::BytesVector buff;
			for (uintmax_t i = 0; i < header.dataLen;)
			{
				Compression::unit u = _inputFile.Get();
				if (d.Next(u, buff))
				{
					i += buff.size();
					file.Write(buff);
					buff.clear();
				}
			}

			return;
		}

	}



	void Decoder::_OpenFile(Path path)
	{
		if (!_inputFile.Open(true, path))
			throw CantOpenFileException(path.u8string().c_str());

	}



	Decoder::~Decoder() {	}

	bool Decoder::VerifyFormat()
	{
		union FMT_Name
		{
			u32 u;
			char c[5]{};
		}fmt;
		fmt.u = _inputFile.Read<u32>();
		return strcmp(FMT_HEADER_, fmt.c) == 0;
	}

	bool Decoder::VerifyVersion()
	{
		u32 v = _inputFile.Read<u32>();
		return FMT_VERSION == v;

	}
}




