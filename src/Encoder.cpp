#include "Encoder.h"
#include "Encoder.h"
#include "Encoder.h"
namespace H4z0t {

	Encoder::Encoder()
	{
		_OpenFile();
	}

	Encoder::Encoder(String)
	{
		_OpenFile();
	}

	Encoder::Encoder(Path)
	{
		_OpenFile();
	}

	Encoder::Encoder(DirEntry)
	{
		_OpenFile();
	}

	void Encoder::_OpenFile()
	{
		this->_outputFile = new std::fstream();
		this->_outputFile->open(String(DEFAULT_OUTPUT_PATH), std::ios::binary | std::ios::out);

		if (!this->_outputFile->is_open())throw std::exception("CANT OPEN FILE");
		//this->_outputFile->imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	}

	void Encoder::MakeHeader()
	{
		assert(this->_outputFile != nullptr && this->_outputFile->is_open(), "file wasnt created and opened");

		this->_outputFile->write(FMT_HEADER_, strlen(FMT_HEADER_))
			.write(reinterpret_cast<const char*>(&FMT_VERSION), sizeof(FMT_VERSION));

	}

	Encoder::~Encoder()
	{

		assert(this->_outputFile != nullptr, "file wasnt created");
		if (this->_outputFile->is_open())this->_outputFile->close();
		delete this->_outputFile;
	}

	Encoder& Encoder::WriteFileHeader(File::Header h)
	{
		return this->Write(h);

	}

	Encoder& Encoder::WriteString(const char* s)
	{

		this->_outputFile->write(s, strlen(s));
		return *this;
	}

	Encoder& Encoder::Put(char c)
	{
		_outputFile->put(c);
		return *this;
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
				WriteString(file.GetName().u8string());
				for (uintmax_t i = 0; i < h.dataLen; i++)
				{
					Put(file.Get());
				}

			}
			else
			{
				std::cerr << "Unable to open file " << f.path() << std::endl;
			}

		}

	}

	Encoder& Encoder::WriteString(String s)
	{

		this->_outputFile->write(s.c_str(), s.length());
		return *this;
	}

	template<typename T>
	Encoder& Encoder::Write(T value)
	{
		this->_outputFile->write(reinterpret_cast<const char*>(&value), sizeof(T));
		return *this;
	}


	void Encoder::MakeHeader(u32 fileCount)
	{
		this->MakeHeader();
		this->Write(fileCount);

	}
}