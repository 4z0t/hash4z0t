
#pragma once
#include "../internalhash4z0t.h"
#include <algorithm>
#include <iostream>
#include "../BitsAndBytes/BitsAndBytes.h"



namespace Encryption
{


	using namespace BitsAndBytes;


	size_t GCD(size_t n1, size_t n2)
	{

		while (n1 != n2)
		{
			if (n1 == 0)
				return  n2;
			if (n2 == 0)
				return n1;
			if (n1 > n2)
				n1 %= n2;
			else
				n2 %= n1;
		}
		return n1;
	}



	bool IsPrime(const size_t v)
	{
		for (size_t i = 2; i <= std::sqrt(v); i++)
		{
			if (v % i == 0)return false;
		}
		return true;
	}


	size_t EulersTotient(size_t p, size_t q)
	{
		return (p - 1) * (q - 1);
	}


	size_t CalcE(size_t t)
	{
		for (size_t e = 2; e < t; e++)
		{
			if (GCD(e, t) == 1)return e;
		}
		throw std::invalid_argument("Unable to calculate open exponent");
	}


	size_t CalcD(size_t e, size_t t)
	{
		size_t k = 1;
		while (k % e != 0)
		{
			k += t;
		}
		return k / e;
	}



	size_t PowMod(const size_t v, size_t n, size_t mod)
	{
		size_t result = 1;
		size_t buffer = v % mod;

		for (size_t i = 0; i < sizeof(n) * 8 && (1ull << i) <= n; i++)
		{
			if (n & (1ull << i))
			{
				result *= buffer;
				result %= mod;
			}
			if ((1ull << i) < n)
			{
				buffer *= buffer;
				buffer %= mod;
			}
		}
		return result;
	}

	/*size_t PowMod(const size_t v, size_t n, size_t mod)
	{
		size_t result = 1;


		for (size_t i = 0; i < n; i++)
		{
			result *= v;
			result %= mod;
		}
		return result;
	}*/





	using UIntVector = std::vector<size_t>;

	UIntVector BytesToUInts(const BytesVector& v)
	{
		UIntVector res(v.size() / sizeof(size_t) + ((v.size() % sizeof(size_t) == 0) ? 0 : 1));
		size_t buffer;

		for (size_t i = 0; i < v.size(); i++)
		{
			buffer = v[i];
			res[i / sizeof(size_t)] |= buffer << ((i % sizeof(size_t)) * 8);
		}
		return res;
	}

	BytesVector UIntsToBytes(const UIntVector& v)
	{
		BytesVector res(v.size() * sizeof(size_t));
		size_t buffer;

		for (size_t i = 0; i < res.size(); i++)
		{
			res[i] = v[i / sizeof(size_t)] >> ((i % sizeof(size_t)) * 8);
		}
		return res;
	}


	namespace RSA
	{
		using byte = uint8_t;


		size_t Encode(const size_t v, size_t e, size_t n)
		{
			return PowMod(v, e, n);
		}


		size_t Decode(const size_t v, size_t d, size_t n)
		{
			return PowMod(v, d, n);
		}



		BytesVector Encode(const BytesVector& v, size_t e, size_t n)
		{
			UIntVector res(v.size());
			std::cout << "encoding: " << std::endl;
			for (size_t i = 0; i < v.size(); i++)
			{
				std::cout << (int)v[i] << " -> ";
				size_t s = Encode(v[i], e, n);
				std::cout << s << ", ";
				res[i] = s;
			}
			std::cout << std::endl;
			return UIntsToBytes(res);

		}


		BytesVector Decode(const BytesVector& v, size_t d, size_t n)
		{
			UIntVector buffer = BytesToUInts(v);
			BytesVector res(buffer.size());
			std::cout << "decoding: " << std::endl;
			for (size_t i = 0; i < buffer.size(); i++)
			{
				std::cout << buffer[i] << " -> ";
				size_t s = Decode(buffer[i], d, n);
				std::cout << s << ", ";
				res[i] = s;
			}
			std::cout << std::endl;
			return res;
		}




	}



}