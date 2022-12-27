#pragma once
#include "compression.h"
#include <algorithm>


namespace Compression
{

#if false
#define __debug_log(exp) std::cout << exp <<std::endl;
#else
#define __debug_log(exp) (void)0;
#endif

#if false
	void DisplayCodes(const std::vector<unit_normalized>& norm)
	{
		for (const auto& k : norm)
		{
			std::cout << "[" << (int)k.first << "]\t" << (int)k.second << std::endl;
		}
		std::cout << std::endl;
	}
#else
	void DisplayCodes(const std::vector<unit_normalized>& norm) {}
#endif
#if false
	void DisplayCodes(const std::unordered_map<unit, Code>& map)
	{
		for (const auto& k : map)
		{
			std::cout << "[" << (int)k.first << "]\t";
			for (auto v : k.second)
			{
				std::cout << (int)v;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
#else
	void DisplayCodes(const std::unordered_map<unit, Code>& map) {}
#endif

	class FrequencyTable
	{
	public:
		FrequencyTable(const BytesVector& data)
		{
			_total = data.size();
			this->_Count(data);
			this->_Sort();


		}

		FrequencyTable(const std::vector<unit_normalized>& data)
		{
			_total = data.size();
			_Restore(data);
			_Sort();
		}

		FrequencyTable(const std::unordered_map<unit, size_t>& data)
		{


			size_t max_unit_count = 0;
			for (const auto& k : data)
			{
				max_unit_count = std::max(max_unit_count, k.second);
			}


			std::transform(
				data.begin(),
				data.end(),
				std::back_inserter(this->_data),
				[max_unit_count](const unit_count& uc)
				{
					return unit_frequency(uc.first, ((double)uc.second / max_unit_count));
				}
			);
			_Sort();
		}



		const std::vector<unit_frequency>& Get()const
		{
			return _data;
		}





		std::vector<unit_normalized> GetScaled()const
		{
			std::vector<unit_normalized>res;

			std::transform(
				_data.begin(),
				_data.end(),
				std::back_inserter(res),
				[](const unit_frequency& uc)
				{
					return  unit_normalized(uc.first, (unit)(uc.second * 254 + 1));//TODO
				}
			);
			return res;
		}

		size_t GetTotal()const
		{
			return _total;
		}
	private:

		void _Restore(const std::vector<unit_normalized>& data)
		{
			_data.reserve(data.size());

			size_t norm_sum = 0;
			for (const auto& un : data)
			{
				norm_sum += un.second;
			}

			for (const auto& un : data)
			{
				_data.emplace_back(un.first, (double)un.second / norm_sum);
			}
		}

		void _Count(const BytesVector& data)
		{
			std::unordered_map<unit, size_t> _data;



			for (size_t i = 0; i < data.size(); i++)
			{
				if (_data.find(data[i]) == _data.end())
				{
					_data[data[i]] = 1;
				}
				else
				{
					_data[data[i]] += 1;
				}
			}

			size_t max_unit_count = 0;
			for (const auto& k : _data)
			{
				max_unit_count = std::max(max_unit_count, k.second);
			}


			std::transform(
				_data.begin(),
				_data.end(),
				std::back_inserter(this->_data),
				[max_unit_count](const unit_count& uc)
				{
					return unit_frequency(uc.first, ((double)uc.second / max_unit_count));
				}
			);


		}


		void _Sort()
		{
			std::sort(
				_data.begin(), _data.end(),
				[](const unit_frequency& a, const unit_frequency& b)
				{
					if (a.second == b.second)
						return a.first > b.first;
					return a.second > b.second;
				}
			);
		}






	private:
		std::vector<unit_frequency> _data;

		size_t _total;
	};



	bool MatchCode(const std::unordered_map<unit, Code>& map, const BitsVector& bits, unit& output)
	{
		for (const auto& ucode : map)
		{
			if (ucode.second == bits)
			{
				output = ucode.first;
				return true;
			}
		}
		return false;
	}

	namespace ShannonFano
	{
		std::unordered_map<unit, Code > FrequencyToCodes(const FrequencyTable& ft);



		class Decoder
		{
		public:
			Decoder() {};
			~Decoder() {};

			bool Next(unit u, BytesVector& res)
			{
				if (_dictSize == 0)
				{
					_dictSize = (size_t)u + 1;
					__debug_log("size: " << (int)_dictSize);
					_head.reserve(_dictSize * 2);
					return false;
				}

				if (!_headDone)
				{
					_head.push_back(u);
					if (_head.size() == _dictSize * 2)
					{
						std::vector<unit_normalized> norm;
						norm.reserve(_dictSize);
						for (size_t i = 0; i < _head.size(); i += 2)
						{
							norm.emplace_back(_head[i], _head[i + 1]);
						}
						FrequencyTable f(norm);
						DisplayCodes(norm);
						_codes = FrequencyToCodes(f);
						DisplayCodes(_codes);
						_headDone = true;
						_head.clear();
					}
					return false;
				}

				if (_len == 0)
				{
					_head.push_back(u);
					if (_head.size() == sizeof(size_t))
					{
						_len = ReadBytes<size_t>(_head, 0);
						_head.clear();
					}
					return false;
				}


				PushBytes(_buffer, u);

				return Match(res);

			}


			size_t GetMinCodeSize()
			{
				size_t min_len = 256;
				for (const auto& k : _codes)
				{
					if (min_len > k.second.size())
					{
						min_len = k.second.size();
					}
				}
				return min_len;
			}

			bool Match(BytesVector& res)
			{
				if (curIndex == _len)  return false;
				bool found = false;
				BitsVector buff;
				unit u;
				size_t last_index;

				size_t min_len = GetMinCodeSize();

				for (size_t i = 0; i < _buffer.size(); i++)
				{
					buff.push_back(_buffer[i]);
					if (buff.size() >= min_len && MatchCode(_codes, buff, u))
					{
						res.push_back(u);
						curIndex++;
						buff.clear();
						last_index = i;
						found = true;
						if (curIndex == _len)break;
					}
				}
				if (found)
				{
					ShiftLeft(_buffer, last_index + 1);
				}
				return found;
			}


		private:
			bool _headDone = false;
			size_t _len = 0;
			BytesVector _head;

			BitsVector _buffer;
			size_t _dictSize = 0;
			std::unordered_map<unit, Code > _codes;
			size_t curIndex = 0;
		};



		bool PullBytesFromFront(BitsVector& bits, BytesVector& bytes)
		{
			auto size = bits.size();
			if (size >= 8)
			{
				BitsVector r(bits.begin(), bits.begin() + (size / 8) * 8);
				bytes = ToBytes(r);
				ShiftLeft(bits, (size / 8) * 8);
				return true;
			}
			return false;
		}



		void Split
		(
			std::unordered_map<unit, Code>& unitsToCodes,	// unit -> code
			const std::vector<unit_frequency>& frequency,	// unit and frequency
			size_t start,									// start index
			size_t end,										// end index
			double threshold								// threshold to divide groups
		)
		{
			switch (end - start)
			{
			case 2:
				unitsToCodes[frequency[start + 1].first].push_back(true);
				__fallthrough;
			case 1:
				unitsToCodes[frequency[start].first].push_back(false);
				__fallthrough;
			case 0:
				return;
			default:
				break;
			}
			size_t middle = start;
			double passedThreshold = 0;
			{
				double c = 0;
				double prev;
				bool passed = false;
				for (auto i = start; i < end; i++)
				{
					auto key = frequency[i].first;
					prev = c;
					c += frequency[i].second;
					if (!passed && (c > threshold || frequency[i].second == 0.0 && i >= (start + end) / 2))
					{
						if ((c - threshold) > (threshold - prev))
						{
							passedThreshold = prev;
							//current one is edge
							unitsToCodes[key].push_back(true);
							middle = i;
						}
						else
						{
							passedThreshold = c;
							//next one is edge
							unitsToCodes[key].push_back(false);
							middle = i + 1;
						}
						passed = true;
						continue;
					}
					unitsToCodes[key].push_back(passed);
				}
			}
			auto t1 = passedThreshold / 2;
			auto t2 = threshold - t1;
			if (middle == start)middle++;
			Split(unitsToCodes, frequency, start, middle, t1);
			Split(unitsToCodes, frequency, middle, end, t2);
		}

		std::unordered_map<unit, Code > FrequencyToCodes(const FrequencyTable& ft)
		{
			std::unordered_map<unit, Code> unitsToCodes;
			const auto& v = ft.Get();
			for (auto& p : v)
			{
				unitsToCodes[p.first] = std::vector<bool>();
			}
			Split(unitsToCodes, v, 0, v.size(), 0.5);
			return unitsToCodes;
		}



		BitsVector MakeHead(const std::vector<unit_normalized>& unitsToNorm)
		{

			BitsVector head;
			__debug_log("unitsToNorm:" << unitsToNorm.size());
			uint8_t dictSize = unitsToNorm.size() - 1;
			head.reserve((size_t)dictSize + 1);
			uint8_t codeSize = 0;
			PushBytes(head, dictSize);//size of a dict
			for (const auto& uc : unitsToNorm)
			{
				PushBytes(head, uc.first);
				PushBytes(head, uc.second);
			}

			return head;
		}



		_NODISCARD
			BitsVector	Compress(const BytesVector& input)
		{
			FrequencyTable f(input);
			FrequencyTable ft_norm(f.GetScaled());
			std::unordered_map<unit, Code> unitsToCodes = FrequencyToCodes(ft_norm);
			auto codes_norm = f.GetScaled();
			BitsVector msg = MakeHead(codes_norm);
			PushBytes(msg, f.GetTotal());

			DisplayCodes(unitsToCodes);

			for (size_t i = 0; i < input.size(); i++)
			{
				const auto& code = unitsToCodes[input[i]];
				msg.insert(msg.end(), code.begin(), code.end());
			}
			return msg;
		}


		_NODISCARD
			BytesVector Decompress(const BytesVector& input)
		{
			Decoder d;
			BytesVector res;
			res.reserve(input.size());
			BytesVector buff;
			unit u;
			for (size_t i = 0; i < input.size(); i++)
			{
				if (d.Next(input[i], buff))
				{
					res.insert(res.end(), buff.begin(), buff.end());
					buff.clear();
				}
			}
			return res;
		}



	}


}