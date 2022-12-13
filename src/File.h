#pragma once
#include "internalhash4z0t.h"

namespace H4z0t {



	
	class File
	{


	public:
		static struct Header
		{
			uintmax_t dataLen;
			u32 nameLen;
			CompressionType comp = CompressionType::None;
			EncryptionType enc = EncryptionType::None;
			ProtectionType prot = ProtectionType::None;
		};

		File(const Path& path, const Path& relative) :_path(path), _relative(relative)
		{

		}

		File(const Path& path) :_path(path), _relative()
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

		bool Open(bool read)
		{
			assert(_file == nullptr);
			_file = new std::fstream();
			_file->open(_path, (read ? std::ios::in : std::ios::out) | std::ios::binary);

			if (_file->is_open())
			{
				return true;

			}
			else
			{
				delete _file;
				_file = nullptr;
				return false;
			}
		}

		char Get()
		{
			assert(_file != nullptr);
			return _file->get();
		}

		void Put(char c)
		{
			assert(_file != nullptr);
			_file->put(c);
		}

		~File()
		{
			if (_file != nullptr)
			{
				_file->close();
				delete _file;
			}
		}



	private:
		Path _path;
		Path _relative;

		std::fstream* _file = nullptr;


	};

}