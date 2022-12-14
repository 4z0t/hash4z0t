#pragma once
#include <stdint.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <assert.h>
#include <exception>
#include <locale>
#include <codecvt>
#include <vector>
#include <list>
#include <iostream>

namespace H4z0t
{
	namespace FS = std::filesystem;
	using String = std::string;
	using Path = FS::path;
	using DirEntry = FS::directory_entry;

	typedef uint32_t u32;
	enum class DataType
	{
		Folder,
		File
	};

	enum class CompressionType :u32
	{
		None,
		SF,
		LZ77

	};

	enum class EncryptionType :u32
	{
		None
	};

	enum class ProtectionType :u32
	{
		None,
		Hamming,
	};


	const u32 FMT_VERSION = 1;
#define FMT_HEADER_ "4z0t"
#define DEFAULT_OUTPUT_PATH ("output." FMT_HEADER_)


	struct Header
	{


	};

	using Files = std::list<DirEntry>;
	Files GetFileList(const Path& path);




}