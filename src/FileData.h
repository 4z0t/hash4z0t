#pragma once
#include "File.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <array>

namespace H4z0t
{
	
	class FileData
	{
	public:
		using unit = uint8_t;
		FileData() {};


		void Collect(const Path& path)
		{
			File file(path);
			if (!file.Open(true)) throw CantOpenFileException(path.u8string().c_str());
			uintmax_t size = file.GetSize();
			for (uintmax_t i = 0; i < size; i++)
			{
				unit c = file.Get();
				_data[c] += 1;
			}
			file.Close();
		}

		std::unordered_map<unit, size_t> GetData()const
		{
			std::unordered_map<unit, size_t> res;
			for (size_t i = 0; i < _data.size(); i++)
			{
				if (_data[i] != 0)
					res[i] = _data[i];
			}
			return res;
		}


		unit GetLessCharInFile()
		{
			unit c;
			size_t count = std::numeric_limits<size_t>().max();
			for (size_t i = 0; i < _data.size(); i++)
			{
				if (_data[i] < count)
				{
					count = _data[i];
					c = i;
				}
			}
			return c;
		}


		~FileData() {};
	private:
		std::array<size_t, 256> _data{};
	};




}

