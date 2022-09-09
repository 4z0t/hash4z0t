#include "Encoder.h"

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
}

void Encoder::MakeHeader()
{
	assert(this->_outputFile != nullptr && this->_outputFile->is_open(), "file wasnt created and opened");

	this->_outputFile->write(FMT_HEADER_, strlen(FMT_HEADER_))
		.write(reinterpret_cast<const char*>(&FMT_VERSION), sizeof(uint32_t));



}
