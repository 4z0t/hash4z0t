#pragma once
#include "internalhash4z0t.h"
#include <sstream>
namespace H4z0t {
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

		std::string ReadString(u32 len)
		{
			std::stringstream ss;
			for (u32 i = 0; i < len; i++)
				ss.put(this->_inputFile->get());
			return ss.str();
		}

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


}