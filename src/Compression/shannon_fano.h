#pragma once
#include "compression.h"


namespace Compression
{


	class FrequencyTable
	{
	public:
		FrequencyTable(const BytesVector& data)
		{
			_total = data.size();
			this->_Count(data);
			this->_Sort();
		}



		const std::vector<unit_frequency>& Get()const
		{
			return _data;
		}

		const std::unordered_map<unit, size_t>& GetMap()const
		{
			return _unitCount;
		}

		size_t GetTotal()const
		{
			return _total;
		}
	private:
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

			auto total = this->_total;
			std::transform(
				_data.begin(),
				_data.end(),
				std::back_inserter(this->_data),
				[total](const unit_count& uc)
				{
					return unit_frequency(uc.first, (double)uc.second / total);
				}
			);
			_unitCount = std::move(_data);
		}


		void _Sort()
		{
			std::sort(
				_data.begin(), _data.end(),
				[](const unit_frequency& a, const unit_frequency& b)
				{
					return a.second > b.second;
				}
			);
		}




	private:
		std::vector<unit_frequency> _data;
		std::unordered_map<unit, size_t> _unitCount;;
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
					if (!passed && c > threshold)
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
			Split(unitsToCodes, frequency, start, middle, t1);
			Split(unitsToCodes, frequency, middle, end, t2);
		}

		std::unordered_map<unit, Code > FrequencyToCodes(const FrequencyTable& ft)
		{
			std::unordered_map<unit, Code> unitsToCodes;
			auto& v = ft.Get();
			for (auto& p : v)
			{
				unitsToCodes[p.first] = std::vector<bool>();
			}

			Split(unitsToCodes, v, 0, v.size(), 0.5);
//#if _DEBUG 
//			for (auto& p : v)
//			{
//
//				std::cout << p.first << "\t[" << (int)(p.first) << "]\t";
//				for (auto& bit : unitsToCodes[p.first])
//				{
//					std::cout << bit;
//				}
//				std::cout << "\t" << p.second << '\n';
//			}
//#endif
			return unitsToCodes;
		}


		BitsVector MakeHead(const std::unordered_map<unit, Code>& unitsToCodes)
		{
			std::vector<bool> head;
			//head.reserve();
			uint8_t dictSize = unitsToCodes.size();
			uint8_t codeSize = 0;
			PushBytes(head, dictSize);//size of a dict
			for (const unit_Code& uc : unitsToCodes)
			{
				codeSize = uc.second.size();
				PushBytes(head, uc.first);											//unit
				PushBytes(head, codeSize);											//code size
				head.insert(head.end(), uc.second.begin(), uc.second.end());		//code itself
			}

			return head;
		}

		//Returns size in bits required for a message to fit
		size_t GetEncodedSize(const FrequencyTable& ft, const std::unordered_map<unit, Code>& unitsToCodes)
		{
			size_t s = 0;
			for (const unit_count& uc : ft.GetMap())
			{
				s += unitsToCodes.at(uc.first).size() * uc.second;
			}
			return s;
		}

		/*
		*  | dict_size:uint8 | original_unit: unit, unit_size:uint8, unit_code : bits[unit_size] | msg_len : size_t | msg : uint8[] |
		*  |                 |                           codes                                   |                                  |
		*  |                        head														 |           body                   |
		*/
		_NODISCARD
			BitsVector	Compress(const BytesVector& input)
		{
			FrequencyTable f(input);
			std::unordered_map<unit, Code> unitsToCodes = FrequencyToCodes(f);

			BitsVector msg = MakeHead(unitsToCodes);
			PushBytes(msg, f.GetTotal());
			auto s = GetEncodedSize(f, unitsToCodes);

			for (size_t i = 0; i < input.size(); i++)
			{
				const auto& code = unitsToCodes[input[i]];
				msg.insert(msg.end(), code.begin(), code.end());
			}
			return msg;
		}


		std::unordered_map<unit, Code > UnpackHead(const BitsVector& bits, size_t& i)
		{
			size_t index = 0;
			std::unordered_map<unit, Code > codes;
			uint8_t dictSize = ReadBytes<uint8_t>(bits, index); index += 8;
			for (uint8_t unitId = 0; unitId < dictSize; unitId++)
			{
				unit unit = ReadBytes<uint8_t>(bits, index); index += 8;
				uint8_t codeLen = ReadBytes <uint8_t>(bits, index); index += 8;
				auto code = ReadBits(bits, index, codeLen); index += codeLen;
				codes[unit] = code;
			}

			i = index;
			return codes;
		}



		_NODISCARD
			BytesVector Decompress(const BitsVector& input)
		{
			size_t index;
			auto ucodes = UnpackHead(input, index);
			size_t size = ReadBytes<size_t>(input, index); index += sizeof(size_t) * 8;
			BitsVector buff;
			unit u;
			BytesVector res;
			res.reserve(size);
			size_t curIndex = 0;
			for (size_t i = index; i < input.size(); i++)
			{
				buff.push_back(input[i]);
				if (MatchCode(ucodes, buff, u))
				{
					curIndex++;
					res.push_back(u);
					buff.clear();
				}
				if (curIndex == size)break;
			}


			return res;
		}



	}


}