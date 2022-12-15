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
			File::Header h = _inputFile.Read<File::Header>();
			std::string name = _inputFile.ReadString(h.nameLen);
			File file(curDir / name);
			FS::create_directories((curDir / name).parent_path());
			if (file.Open(false))
			{
				std::cout << "File opened\t" << name << std::endl;
				for (uintmax_t i = 0; i < h.dataLen; i++)
				{
					file.Put(_inputFile.Read<char>());
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
		if (!_inputFile.Open(true, path))
			throw CantOpenFileException(path.u8string().c_str());

	}



	Decoder::~Decoder() {	}

	bool Decoder::VerifyFormat()
	{
		union FMT_Name
		{
			u32 u;
			char c[5]{};
		}fmt;
		fmt.u = _inputFile.Read<u32>();
		return strcmp(FMT_HEADER_, fmt.c) == 0;
	}

	bool Decoder::VerifyVersion()
	{
		u32 v = _inputFile.Read<u32>();
		return FMT_VERSION == v;

	}
}




