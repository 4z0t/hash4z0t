#include "Decoder.h"
#include "Decoder.h"
Decoder::Decoder()
{
	_OpenFile(std::filesystem::current_path());
}

Decoder::Decoder(std::string s)
{
	_OpenFile(Path(s));
}

Decoder::Decoder(Path path)
{
	_OpenFile(path);
}

Decoder::Decoder(std::filesystem::directory_entry entry)
{
	_OpenFile(entry);
}

void Decoder::_OpenFile(Path path)
{
	this->_inputFile = new std::fstream();
	this->_inputFile->open(path, std::ios::binary | std::ios::in);
	if (!this->_inputFile->is_open())throw std::exception("CANT OPEN FILE");
}

bool Decoder::VerifyFormat()
{
	union FMT_Name
	{
		u32 u;
		char c[5]{};
	}fmt;
	fmt.u = this->Read<u32>();
	return strcmp(FMT_HEADER_, fmt.c) == 0;
}

bool Decoder::VerifyVersion()
{
	u32 v = this->Read<u32>();
	return FMT_VERSION == v;

}




