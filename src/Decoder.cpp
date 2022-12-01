#include "Decoder.h"
namespace H4z0t {
	Decoder::Decoder()
	{
		_OpenFile(std::filesystem::current_path());
	}

	Decoder::Decoder(String s)
	{
		_OpenFile(Path(s));
	}

	Decoder::Decoder(Path path)
	{
		_OpenFile(path);
	}

	Decoder::Decoder(DirEntry entry)
	{
		_OpenFile(entry);
	}

	void Decoder::Start(const String& savePath)
	{

		if (!VerifyFile())
		{
			throw InvaildFileException();
		}

		Path curDir = FS::current_path();
		if (!savePath.empty())
		{
			curDir /= savePath;
		}
		u32 filesCount = FilesCount();


		for (u32 i = 0; i < filesCount; i++)
		{
			File::Header h = Read<File::Header>();
			std::string name = ReadString(h.nameLen);
			File file(curDir / name);
			FS::create_directories((curDir / name).parent_path());
			if (file.Open(false))
			{
				std::cout << "File opened\t" << name << std::endl;
				for (uintmax_t i = 0; i < h.dataLen; i++)
				{
					file.Put(Read<char>());
				}
			}
			else
			{

				std::cout << "File not opened " << name << std::endl;
				throw CantOpenFileException(name.c_str());
			}


		}



	}

	void Decoder::_OpenFile(Path path)
	{
		this->_inputFile = new std::fstream();
		this->_inputFile->open(path, std::ios::binary | std::ios::in);
		if (!this->_inputFile->is_open())throw std::exception("CANT OPEN FILE");
	}

     String Decoder::ReadString(u32 len)
	{
		std::stringstream ss;
		for (u32 i = 0; i < len; i++)
			ss.put(this->_inputFile->get());
		return ss.str();
	}

	Decoder::~Decoder()
	{

		assert(this->_inputFile != nullptr, "file wasnt created");
		if (this->_inputFile->is_open())this->_inputFile->close();
		delete this->_inputFile;
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
}




