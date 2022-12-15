#pragma once

#include "internalhash4z0t.h"
#include "FileRegister.h"
#include "File.h"
namespace H4z0t {


	class Encoder
	{
	public:
		Encoder();
		Encoder(const String&);
		Encoder(const Path&);
		Encoder(const DirEntry&);


		void _OpenFile(const Path&);

		void MakeHeader();
		void MakeHeader(u32 fileCount);


		void WriteFileHeader(File::Header h);



		void Start(const Path& filesPath);


		~Encoder();

	protected:
		File _outputFile;
		Path _workPath;
	};
}