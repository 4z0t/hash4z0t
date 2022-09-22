#pragma once
#include "internalhash4z0t.h"
namespace FS = std::filesystem;
class File
{


public:
	using Path = FS::path;
	static struct Header
	{
		u32 nameLen;
		uintmax_t dataLen;
		Compression comp = Compression::None;
		Encryption enc = Encryption::None;
		Protecion prot = Protecion::None;
	};

	File(const Path& path, const Path& relative) :_path(path), _relative(relative)
	{

	}

	Header	MakeHeader()
	{
		Header h;
		h.nameLen = _relative.u8string().length();
		h.dataLen = FS::directory_entry(_path).file_size();
		return h;
	}

	Path GetName()
	{
		return _relative;
	}




private:
	Path _path;
	Path _relative;


};

