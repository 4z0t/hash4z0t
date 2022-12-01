#pragma once
#include "internalhash4z0t.h"
#include <sstream>
namespace H4z0t {


	class InvaildFileException : public std::exception {};



	class Decoder
	{
	public:
		using String = std::string;
		using Path = std::filesystem::path;
		using DirEntry = std::filesystem::directory_entry;


		Decoder();
		Decoder(String);
		Decoder(Path);
		Decoder(DirEntry);


		bool VerifyFile()
		{
			return VerifyFormat() && VerifyVersion();
		}

		void Start()
		{
			if (!VerifyFile())
			{
				throw InvaildFileException();
			}


		}



		template<typename T>
		T Read();



		std::string ReadString(u32 len);

		~Decoder();

	protected:

		bool VerifyFormat();
		bool VerifyVersion();
		void _OpenFile(Path);


		Path _filePath;
		std::fstream* _inputFile = nullptr;

	};


	template<typename T>
	inline T Decoder::Read()
	{
		T res;
		this->_inputFile->read(reinterpret_cast<char*>(&res), sizeof(T));
		return res;
	}

}