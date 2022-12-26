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
	typedef  std::pair<unit, unit> unit_normalized;
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

	template<typename T>
	T ReadBytes(const BytesVector& v, size_t start)
	{
		union
		{
			T res;
			unit data[sizeof(T)];
		}u;
		for (size_t i = 0; i < sizeof(T); i++)
		{
			u.data[i] = (v[i + start]);
		}
		return u.res;
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

	template<typename T>
	void ShiftLeft(std::vector<T>& vec, size_t shift)
	{
		for (size_t i = 0; i < vec.size() - shift; i++)
		{
			vec[i] = vec[shift + i];
		}
		vec.resize(vec.size() - shift);
	}




	class Bits;
	class Bytes;


	class Bits : private BitsVector
	{
	public:
		using BitsVector::vector;
		using BitsVector::operator=;
		using BitsVector::operator[];

		Bits(const BitsVector& v) :BitsVector(v) {}


		Bits(const BytesVector& v) :BitsVector(v.size() * 8, false)
		{
			for (size_t i = 0; i < v.size(); i++)
			{
				for (size_t b = 0; b < 8; b++)
				{
					this->operator[](i * 8 + b) = (v[i] >> b) & 1;
				}
			}
		}


		template<typename T>
		T GetBytesFrom(size_t start)const
		{
			T res{};
			for (size_t i = 0; i < sizeof(T) * 8; i++)
			{
				res |= (((T)(this->operator[](i + start))) << i);
			}
			return res;
		}


		BitsVector GetBitsFrom(size_t start, size_t size)const
		{
			const auto& _b = begin();
			return BitsVector(_b + start, _b + start + size);
		}




		size_t Size()const noexcept { return size(); }
		void Clear() noexcept { return clear(); }






		template<typename T>
		void PushBytesBack(const T& val)
		{
			for (size_t i = 0; i < sizeof(T) * 8; i++)
			{
				push_back((val >> i) & 1);
			}
		}


		void PushBack(const bool& val) { return push_back(val); }
		




	private:

	};


	class Bytes :private BytesVector
	{
	public:
		using BytesVector::vector;
		using BytesVector::operator=;
		using BytesVector::operator[];




		Bytes(const BytesVector& v) :BytesVector(v) {}


		size_t Size()const noexcept { return size(); }
		void Clear() noexcept { return clear(); }

	private:

	};

}

std::ostream& operator<<(std::ostream& out, const BitsAndBytes::BitsVector& v)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		if (i % 8 == 0 && i != 0)
		{
			out << '.';
		}
		out << (int)v[i];
	}
	return out;
}