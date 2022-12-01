#pragma once
#include <stdint.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <assert.h>
#include <exception>
#include <locale>
#include <codecvt>


namespace H4z0t
{
	using String = std::string;
	using Path = std::filesystem::path;
	using DirEntry = std::filesystem::directory_entry;

	typedef uint32_t u32;
	enum class DataType
	{
		Folder,
		File
	};

	enum class CompressionType :u32
	{
		None
	};

	enum class EncryptionType :u32
	{
		None
	};

	enum class ProtecionType :u32
	{
		None
	};


	const u32 FMT_VERSION = 1;
#define FMT_HEADER_ "4z0t"
#define DEFAULT_OUTPUT_PATH ("output." FMT_HEADER_)


	struct Header
	{


	};
}