#include "Encoder.h"
#include "Encoder.h"
namespace H4z0t {

	Encoder::Encoder()
	{
		_OpenFile();
	}

	Encoder::Encoder(std::string)
	{
		_OpenFile();
	}

	Encoder::Encoder(std::filesystem::path)
	{
		_OpenFile();
	}

	Encoder::Encoder(std::filesystem::directory_entry)
	{
		_OpenFile();
	}

	void Encoder::_OpenFile()
	{
		this->_outputFile = new std::fstream();
		this->_outputFile->open(std::string(DEFAULT_OUTPUT_PATH), std::ios::binary | std::ios::out);

		if (!this->_outputFile->is_open())throw std::exception("CANT OPEN FILE");
		//this->_outputFile->imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	}

	void Encoder::MakeHeader()
	{
		assert(this->_outputFile != nullptr && this->_outputFile->is_open(), "file wasnt created and opened");

		this->_outputFile->write(FMT_HEADER_, strlen(FMT_HEADER_))
			.write(reinterpret_cast<const char*>(&FMT_VERSION), sizeof(FMT_VERSION));

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

	Encoder& Encoder::WriteString(std::string s)
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