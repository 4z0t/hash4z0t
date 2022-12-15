#pragma once
#include "File.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

namespace H4z0t
{

	class FileData
	{
	public:
		FileData() {};


		void Collect(const Path& path)
		{
			File file(path);
			if(!file.Open(true)) throw CantOpenFileException(path.u8string().c_str());

			for (size_t i = 0; i < file.GetSize(); i++)
			{
				char c = file.Get();
				if (_data.find(c) == _data.end())_data[c] = 0;
				_data[c] += 1;
			}
		}

		const std::unordered_map<char, size_t>& GetData()const
		{
			return _data;
		}


		char GetLessCharInFile()
		{
			char c;
			size_t count = std::numeric_limits<size_t>().max();
			for (auto& d: _data)
			{
				if (d.second < count)
				{
					count = d.second;
					c = d.first;
				}
			}
			return c;
		}


		~FileData() {};
	private:
		std::unordered_map<char, size_t> _data;

	};




}

