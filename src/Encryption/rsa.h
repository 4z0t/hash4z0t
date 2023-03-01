
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
		size_t buffer = v;

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




	using UIntVector = std::vector<size_t>;

	UIntVector BytesToUInts(const BytesVector& v)
	{

	}

	BytesVector UIntsToBytes(const UIntVector& v)
	{

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
			BytesVector res(v.size());

			for (size_t i = 0; i < v.size(); i++)res[i] = Encode(v[i], e, n);
			return res;

		}


		BytesVector Decode(const BytesVector& v, size_t d, size_t n)
		{
			BytesVector res(v.size());

			for (size_t i = 0; i < v.size(); i++)res[i] = Decode(v[i], d, n);
			return res;
		}




	}



}