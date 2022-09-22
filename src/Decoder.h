#pragma once
#include "internalhash4z0t.h"

class Decoder
{
public:
	using Path = std::filesystem::path;
	Decoder();
	Decoder(std::string);

	Decoder(Path);
	Decoder(std::filesystem::directory_entry);

	bool VerifyFormat();
	bool VerifyVersion();


	template<typename T>
	T Read()
	{
		T res;
		this->_inputFile->read(reinterpret_cast<char*>(&res), sizeof(T));
		return res;
	}


	void _OpenFile(Path);



	~Decoder()
	{

		assert(this->_inputFile != nullptr, "file wasnt created");
		if (this->_inputFile->is_open())this->_inputFile->close();
		delete this->_inputFile;
	}

protected:
	std::fstream* _inputFile = nullptr;

	Path _filePath;
};


