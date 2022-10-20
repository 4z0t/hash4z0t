#include "../Chunk.h"
#include "../internalhash4z0t.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

namespace Compression
{
	using unit = std::uint8_t;
	using std::size_t;

	struct Data {
		size_t len = 0;
		unit* data = nullptr;

	};

	Data CStringToData(char* s)
	{
		return{ strlen(s),	reinterpret_cast<unit*>(s) };
	}

	using Code = std::vector<bool>;
	typedef  std::pair<unit, size_t> unit_count;
	typedef  std::pair<unit, double> unit_frequency;
	typedef  std::pair<unit, Code> unit_Code;
	using BitsVector = std::vector<bool>;
	using BytesVector = std::vector<uint8_t>;

	class FrequencyTable
	{
	public:
		FrequencyTable(const Data& data)
		{
			_total = data.len;
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
		void _Count(const Data& d)
		{
			std::unordered_map<unit, size_t> _data;

			for (size_t i = 0; i < d.len; i++)
			{
				if (_data.find(d.data[i]) == _data.end())
				{
					_data[d.data[i]] = 1;
				}
				else
				{
					_data[d.data[i]] += 1;
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

	template<typename T>
	void PushBytes(std::vector<bool>& v, const T& val)
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
	BitsVector ReadBits(const std::vector<bool>& v, size_t start, size_t size)
	{
		BitsVector bits(size);
		for (size_t i = 0; i < size; i++)
		{
			bits[i] = v[start + i];
		}
		return bits;
	}

	BytesVector ToBytes(const BitsVector& bits)
	{

		size_t s = bits.size() / 8 + 1;
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
				unitsToCodes[frequency[start].first].push_back(false);
				unitsToCodes[frequency[start + 1].first].push_back(true);
				__fallthrough;
			case 1:
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
#if _DEBUG 
			for (auto& p : v)
			{

				std::cout << p.first << "\t[" << (int)(p.first) << "]\t";
				for (auto& bit : unitsToCodes[p.first])
				{
					std::cout << bit;
				}
				std::cout << "\t" << p.second << '\n';
			}
#endif
			return unitsToCodes;
		}


		/*
		*
		*/
		std::vector<bool> MakeHead(const std::unordered_map<unit, Code>& unitsToCodes)
		{
			std::vector<bool> head;
			//head.reserve();
			uint8_t dictSize = unitsToCodes.size();
			uint8_t codeSize = 0;
			PushBytes(head, dictSize);//size of a dict
			for (const unit_Code& uc : unitsToCodes)
			{
				codeSize = uc.second.size();
#if _DEBUG 
				std::cout << (int)codeSize << std::endl;

				std::cout << uc.first << std::endl;
#endif
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
			BitsVector	Compress(const Data& input)
		{
			FrequencyTable f(input);
			std::unordered_map<unit, Code> unitsToCodes = FrequencyToCodes(f);

			std::vector<bool> msg = MakeHead(unitsToCodes);
			PushBytes(msg, f.GetTotal());
			auto s = GetEncodedSize(f, unitsToCodes);

			for (size_t i = 0; i < input.len; i++)
			{
				const auto& code = unitsToCodes[input.data[i]];
				msg.insert(msg.end(), code.begin(), code.end());
			}
			return msg;
		}

		/*_NODISCARD
			BytesVector Decompress(const Data& input)
		{

			return input;
		}*/




		std::unordered_map<unit, Code > UnpackHead(const BitsVector& bits, size_t& i)
		{
			size_t index = 0;
			std::unordered_map<unit, Code > codes;
			uint8_t dictSize = ReadBytes<uint8_t>(bits, index); index += 8;
			std::cout << (int)dictSize << std::endl;
			for (uint8_t unitId = 0; unitId < dictSize; unitId++)
			{
				uint8_t unit = ReadBytes<uint8_t>(bits, index); index += 8;
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

			return BytesVector();
		}



	}


}