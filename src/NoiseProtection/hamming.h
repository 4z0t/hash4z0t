
#pragma once
#include "../Chunk.h"
#include "../internalhash4z0t.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "../BitsAndBytes/BitsAndBytes.h"

namespace NoiseProtection
{
	using namespace BitsAndBytes;


	constexpr inline size_t Pow2(size_t n)
	{
		return ((size_t)1) << n;
	}

#define POW2CASE(n) case Pow2(n)-1: return (n)


	constexpr size_t IsPow2m1(size_t n)
	{
		switch (n)
		{
			POW2CASE(1);
			POW2CASE(2);
			POW2CASE(3);
			POW2CASE(4);
			POW2CASE(5);
			POW2CASE(6);
			POW2CASE(7);
			POW2CASE(8);
			POW2CASE(9);
			POW2CASE(10);
			POW2CASE(11);
			POW2CASE(12);
			POW2CASE(13);
			POW2CASE(14);
		default:
			return 0;
		}
	}
#undef POW2CASE

	namespace Hamming
	{

		class FlipBitException : public std::exception
		{
			using std::exception::exception;
		};

		constexpr size_t MAX_CHECK_BITS = 8;
		constexpr size_t MAX_BLOCK_LEN = Pow2(MAX_CHECK_BITS) - 1;
		constexpr size_t MAX_MSG_LEN = MAX_BLOCK_LEN - MAX_CHECK_BITS;


		size_t CheckBlockLength(size_t len)
		{
			size_t i = 0;
			while (true)
			{
				size_t expected_len = Pow2(i) - 1;
				if (len < expected_len)	return 0;
				if (len == expected_len) return i;
				i++;
			}
		}


		size_t CalculateCheckBitsCount(size_t msg_len)
		{
			size_t i = 0;
			while (msg_len > Pow2(i) - i - 1)
			{
				i++;
			}
			return i;
		}




		bool CalculateCheckBit(const BitsVector& block, size_t position)
		{
			size_t check = 0;
			size_t i = position - 1;
			while (i < block.size())
			{
				for (size_t j = i; j < i + position; j++)
				{
					if (block[j] && IsPow2m1(j) == 0)check++;
				}
				i += 2 * position;
			}
			return check % 2 != 0;
		}


		BitsVector EncodeMessage(const BitsVector& msg)
		{
			size_t check_bits = CalculateCheckBitsCount(msg.size());
			BitsVector output(Pow2(check_bits) - 1, false);
			{
				size_t j = 0;
				size_t k = 0;

				for (size_t i = 0; i < output.size(); i++)
				{

					if (i == (Pow2(k) - 1)) {
						k++;
						continue;
					}

					if (j == msg.size())
					{
						output[i] = 0;
					}
					else
					{
						output[i] = msg[j];
						j++;
					}

				}
			}


			for (size_t i = 0; i < check_bits; i++)
			{
				size_t position = Pow2(i);

				output[position - 1] = CalculateCheckBit(output, position);
			}

			return output;
		}


		BitsVector DecodeBlock(BitsVector block)
		{
			if (block.size() != MAX_BLOCK_LEN)block.resize(MAX_BLOCK_LEN);
			size_t check_bits = CheckBlockLength(block.size());
			assert(check_bits != 0);
			BitsVector output(block.size() - check_bits);




			for (size_t i = 0; i < check_bits; i++)
			{
				size_t position = Pow2(i);

				if (block[position - 1] != CalculateCheckBit(block, position)) throw FlipBitException();
			}


			{
				size_t j = 0;
				size_t k = 0;
				for (size_t i = 0; i < block.size(); i++)
				{
					if (i == (Pow2(k) - 1)) {
						k++;
					}
					else
					{
						output[j] = block[i];
						j++;
					}
				}
			}
			return output;
		}


		BitsVector ToHamming(const BitsVector& input)
		{
			BitsVector output;
			output.reserve(input.size() * MAX_BLOCK_LEN / MAX_MSG_LEN);

			size_t index = 0;

			while (true)
			{
				if (input.size() - index > MAX_MSG_LEN)
				{
					BitsVector block = EncodeMessage(BitsVector(input.begin() + index, input.begin() + index + MAX_MSG_LEN));
					output.insert(output.end(), block.begin(), block.end());
					index += MAX_MSG_LEN;
				}
				else
				{
					BitsVector block = EncodeMessage(BitsVector(input.begin() + index, input.end()));
					output.insert(output.end(), block.begin(), block.end());
					break;
				}
			}


			return output;

		}

		BitsVector FromHamming(const BitsVector& input, size_t len=0)
		{
			BitsVector output;

			output.reserve(input.size() * MAX_MSG_LEN / MAX_BLOCK_LEN);

			size_t index = 0;

			while (true)
			{
				if (input.size() - index > MAX_BLOCK_LEN)
				{
					BitsVector block = DecodeBlock(BitsVector(input.begin() + index, input.begin() + index + MAX_BLOCK_LEN));
					output.insert(output.end(), block.begin(), block.end());
					index += MAX_MSG_LEN;
				}
				else
				{
					BitsVector block = DecodeBlock(BitsVector(input.begin() + index, input.end()));
					output.insert(output.end(), block.begin(), block.end());
					break;
				}
			}
			if(len!=0)output.resize(len);
			return output;

		}

		size_t FindFlip(const BitsVector& block)
		{
			size_t check_bits = CheckBlockLength(block.size());
			size_t pos = 0;
			for (size_t i = 0; i < check_bits; i++)
			{
				size_t position = Pow2(i);

				pos |= static_cast<size_t> (block[position - 1] != CalculateCheckBit(block, position)) << i;
			}
			return pos - 1;
		}
	}
}