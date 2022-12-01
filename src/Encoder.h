#pragma once

#include "internalhash4z0t.h"
#include "FileRegister.h"
#include "File.h"
namespace H4z0t {


	class Encoder
	{
	public:
		Encoder();
		Encoder(String);
		Encoder(Path);
		Encoder(DirEntry);


		void _OpenFile();

		void MakeHeader();
		void MakeHeader(u32 fileCount);

		template<typename T>
		Encoder& Write(T value);


		Encoder& WriteFileHeader(File::Header h);

		Encoder& WriteString(String);
		Encoder& WriteString(const char*);
		Encoder& Put(char c);

		void Start(const Path& filesPath);


		~Encoder();

	protected:
		std::fstream* _outputFile = nullptr;

		Path _workPath;



	};


}