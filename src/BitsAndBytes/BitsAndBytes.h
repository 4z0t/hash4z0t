#pragma once
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>



namespace BitsAndBytes
{
	using unit = std::uint8_t;
	using std::size_t;

	struct Data {
		size_t len = 0;
		unit* data = nullptr;

	};

	Data CStringToData(char* s)
	{
		return{ strlen(s) + 1,	reinterpret_cast<unit*>(s) };
	}

	using Code = std::vector<bool>;
	typedef  std::pair<unit, size_t> unit_count;
	typedef  std::pair<unit, double> unit_frequency;
	typedef  std::pair<unit, Code> unit_Code;
	using BitsVector = std::vector<bool>;
	using BytesVector = std::vector<uint8_t>;


	template<typename T>
	void PushBytes(BitsVector& v, const T& val)
	{
		for (size_t i = 0; i < sizeof(T) * 8; i++)
		{
			v.push_back((val >> i) & 1);
		}
	}

	template<typename T>
	T ReadBytes(const BitsVector& v, size_t start)
	{
		T res{};
		for (size_t i = 0; i < sizeof(T) * 8; i++)
		{

			res |= (((T)(v[i + start])) << i);
		}
		return res;
	}

	BitsVector ReadBits(const BitsVector& v, size_t start, size_t size)
	{
		return BitsVector(v.begin() + start, v.begin() + start + size);
	}

	BytesVector ToBytes(const BitsVector& bits)
	{

		size_t s = bits.size() / 8 + (((bits.size() % 8) == 0) ? 0 : 1);
		BytesVector res(s, 0);
		for (size_t i = 0; i < bits.size(); i++)
		{
			res[i / 8] |= (((uint8_t)bits[i]) << (i % 8));
		}

		return res;
	}

	BitsVector ToBits(const BytesVector& bytes)
	{

		size_t s = bytes.size() * 8;
		BitsVector res(s, false);
		for (size_t i = 0; i < bytes.size(); i++)
		{
			for (size_t b = 0; b < 8; b++)
			{
				res[i * 8 + b] = (bytes[i] >> b) & 1;
			}
		}

		return res;
	}
}