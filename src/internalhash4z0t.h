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
	using std::vector;
	namespace FS = std::filesystem;
	using String = std::string;
	using Path = FS::path;
	using DirEntry = FS::directory_entry;


	class InvaildFileException : public std::exception {};
	class CantOpenFileException : public std::exception { public: using std::exception::exception; };

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

	enum class Mode
	{
		None,
		Decode,
		Encode,
		NotEnoughArguments,
		IncorrectArguments
	};

	struct Arguments
	{
		Mode mode = Mode::None;
		vector<Path> targets;
		Path savePath;
		CompressionType compression = CompressionType::None;
		ProtectionType protection = ProtectionType::None;
		EncryptionType encryption = EncryptionType::None;
	};

	using Files = std::list<DirEntry>;
	Files GetFileList(const Path& path);




}