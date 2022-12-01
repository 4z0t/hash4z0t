#pragma once

#include "internalhash4z0t.h"
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

		void Start(const Path& filesPath)
		{
			auto files = FileRegister::GetFileList(filesPath);
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


		~Encoder();

	protected:
		std::fstream* _outputFile = nullptr;

		Path _workPath;



	};


}