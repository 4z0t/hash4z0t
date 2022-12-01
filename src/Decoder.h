#pragma once
#include "internalhash4z0t.h"
#include "File.h"
#include <sstream>
namespace H4z0t {


	class InvaildFileException : public std::exception {};
	class CantOpenFileException : public std::exception { public: using std::exception::exception; };


	class Decoder
	{
	public:



		Decoder();
		Decoder(String);
		Decoder(Path);
		Decoder(DirEntry);


		bool VerifyFile()
		{
			return VerifyFormat() && VerifyVersion();
		}

		void Start(const String& savePath);







		template<typename T>
		T Read();
		String ReadString(u32 len);


		~Decoder();
	protected:

		u32 FilesCount()
		{
			return Read<u32>();
		}
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