#pragma once
#include "internalhash4z0t.h"
#include <sstream>

namespace H4z0t {




	class File
	{


	public:
		struct Header
		{
			uintmax_t dataLen;
			u32 nameLen;
			CompressionType comp = CompressionType::None;
			EncryptionType enc = EncryptionType::None;
			ProtectionType prot = ProtectionType::None;
		};

		File(const Path& path, const Path& relative) :_path(path), _relative(relative) {}

		File(const Path& path) :_path(path), _relative() {}


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

		inline char Get()
		{
			assert(_file != nullptr);
			return _file->get();
		}

		inline void Put(char c)
		{
			assert(_file != nullptr);
			_file->put(c);
		}

		template<typename T>
		void Write(const T& value)
		{
			assert(_file != nullptr);
			_file->write(reinterpret_cast<const char*>(&value), sizeof(T));
		}

		template<>
		void Write(const String& s)
		{
			assert(_file != nullptr);
			_file->write(s.c_str(), s.length());
		}


		template<typename T>
		T Read()
		{
			assert(_file != nullptr);
			T res;
			_file->read(reinterpret_cast<char*>(&res), sizeof(T));
			return res;
		}

		void Close()
		{
			assert(_file != nullptr);
			if (_file->is_open())
				_file->close();
			_Free();
		}


		String ReadString(size_t len)
		{
			std::stringstream ss;
			for (size_t i = 0; i < len; i++)
				ss.put(Get());
			return ss.str();
		}

		~File()
		{
			if (_file == nullptr)return;
			if (_file->is_open())
				_file->close();
			_Free();
		}



	private:



		void _Free()
		{
			if (_file == nullptr)return;
			delete _file;
			_file = nullptr;
		}



		Path _path;
		Path _relative;

		std::fstream* _file = nullptr;


	};

}