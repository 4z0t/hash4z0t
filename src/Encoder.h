#pragma once

#include "internalhash4z0t.h"

#include "FileRegister.h"
#include "File.h"
#include "FileData.h"


namespace H4z0t 
{


	class Encoder
	{
	public:
		Encoder();
		Encoder(const String&);
		Encoder(const Path&);
		Encoder(const DirEntry&);
		Encoder(const Arguments& args);


		void _OpenFile(const Path&);

		void MakeHeader(u32 fileCount);


		void WriteFileHeader(File::Header h);



		void Start(const Path& filesPath);

		void ProcessFile(File&);

		~Encoder();

	protected:
		File _outputFile;
		Path _workPath;
		CompressionType compression = CompressionType::None;
		ProtectionType protection = ProtectionType::None;
		EncryptionType encryption = EncryptionType::None;

	};
}


#include "Compression/lz77.h"
#include "Compression/shannon_fano.h"
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

	

	void Encoder::MakeHeader(u32 fileCount)
	{
		Header head{};
		head.files_count = fileCount;
		_outputFile.Write(head);
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


		if (compression == CompressionType::None)
		{
			File::Header h = file.MakeHeader();
			h.comp = compression;
			h.enc = encryption;
			h.prot = protection;
			WriteFileHeader(h);
			_outputFile.Write(file.GetName().u8string());
			std::cout << "Encoding with plain code" << std::endl;
			for (uintmax_t i = 0; i < h.dataLen; i++)
			{
				_outputFile.Put(file.Get());
			}
			return;
		}

		if (compression == CompressionType::LZ77)
		{
			FileData file_data;
			file_data.Collect(file.GetPath());
			File::Header h = file.MakeHeader();

			h.comp = compression;
			h.enc = encryption;
			h.prot = protection;

			uint8_t c = file_data.GetLessCharInFile();
			h.data[0] = c;

			WriteFileHeader(h);
			_outputFile.Write(file.GetName().u8string());


			Compression::BytesVector cache;
			cache.reserve(3);

			Compression::LZ77::SlidingWindow window;
			window.SetRefUnit(c);


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
			File::Header h = file.MakeHeader();
			h.comp = compression;
			h.enc = encryption;
			h.prot = protection;
			WriteFileHeader(h);
			_outputFile.Write(file.GetName().u8string());

			std::cout << "Encoding with Shannon Fano" << std::endl;
			FileData file_data;
			file_data.Collect(file.GetPath());
			const auto data = file_data.GetData();

			Compression::FrequencyTable f(data);
			const auto codes_norm = f.GetScaled();
			Compression::FrequencyTable ft_norm(codes_norm);
			const auto unitsToCodes = Compression::ShannonFano::FrequencyToCodes(ft_norm);
			Compression::BitsVector msg = Compression::ShannonFano::MakeHead(codes_norm);
			Compression::PushBytes<size_t>(msg, h.dataLen);
			Compression::BytesVector cache;

			for (uintmax_t i = 0; i < h.dataLen; i++)
			{
				if (Compression::ShannonFano::PullBytesFromFront(msg, cache))
				{
					_outputFile.Write(cache);
					cache.clear();
				}

				Compression::unit u = file.Get();
				const auto& code = unitsToCodes.at(u);
				msg.insert(msg.end(), code.begin(), code.end());
			}

			_outputFile.Write(Compression::ToBytes(msg));

			return;
		}

	}





	Encoder::~Encoder() {}
}