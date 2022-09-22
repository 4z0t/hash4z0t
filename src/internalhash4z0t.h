#pragma once
#include <stdint.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <assert.h>
#include <exception>
#include <locale>
#include <codecvt>


typedef uint32_t u32;
enum class DataType
{
	Folder,
	File
};

enum class Compression:u32
{
	None
};

enum class Encryption :u32
{
	None
};

enum class Protecion :u32
{
	None
};


const u32 FMT_VERSION = 1;
#define FMT_HEADER_ "4z0t"
#define DEFAULT_OUTPUT_PATH ("output." FMT_HEADER_)


struct Header
{
	

};