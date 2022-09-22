#pragma once

#include "internalhash4z0t.h"
#include "File.h"

	class Encoder
	{
	public:
		Encoder();
		Encoder(std::string);
		Encoder(std::filesystem::path);
		Encoder(std::filesystem::directory_entry);


		void _OpenFile();

		void MakeHeader();
		void MakeHeader(u32 fileCount);

		template<typename T>
		Encoder& Write(T value);

		~Encoder()
		{

			assert(this->_outputFile != nullptr, "file wasnt created");
			if (this->_outputFile->is_open())this->_outputFile->close();
			delete this->_outputFile;
		}

		Encoder& WriteFileHeader(File::Header h);

		Encoder& WriteString(std::string);
		Encoder& WriteString(const char*);
		Encoder& Put(char c)
		{
			_outputFile->put(c);
			return *this;
		}

	protected:
		std::fstream* _outputFile = nullptr;

		std::filesystem::path _workPath;



	};


