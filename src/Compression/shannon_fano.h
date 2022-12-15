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

		FrequencyTable(const std::vector<unit_normalized>& data)
		{
			_total = data.size();
			_Restore(data);
			_Sort();

		}



		const std::vector<unit_frequency>& Get()const
		{
			return _data;
		}

		const std::unordered_map<unit, size_t>& GetMap()const
		{
			return _unitCount;
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
					return  unit_normalized(uc.first, uc.second * 255);
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
			for (const auto& un : data)
			{
				_data.emplace_back(un.first, (double)un.second / 255);
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

			auto total = this->_total;
			std::transform(
				_data.begin(),
				_data.end(),
				std::back_inserter(this->_data),
				[total](const unit_count& uc)
				{
					return unit_frequency(uc.first, ((double)uc.second / total));
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
					if (a.second == b.second)
						return a.first > b.first;
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
					_head.reserve(_dictSize * 2);
					_dictSize = u;
					return false;
				}

				if (!_headDone && _head.size() != _dictSize * 2)
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
						_codes = FrequencyToCodes(f);
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

			bool Match(BytesVector& res)
			{
				if (curIndex == _len)  return false;
				bool found = false;
				BitsVector buff;
				unit u;
				size_t last_index;
				for (size_t i = 0; i < _buffer.size(); i++)
				{
					buff.push_back(_buffer[i]);
					if (MatchCode(_codes, buff, u))
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
					_buffer = BitsVector(_buffer.begin() + last_index + 1, _buffer.end());
				}
				return found;
			}


		private:
			bool _headDone = false;
			size_t _len = 0;
			BytesVector _head;

			BitsVector _buffer;
			uint8_t _dictSize = 0;
			std::unordered_map<unit, Code > _codes;
			size_t curIndex = 0;
		};




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
			FrequencyTable ft_norm(ft.GetScaled());
			auto& v = ft_norm.Get();
			for (auto& p : v)
			{
				unitsToCodes[p.first] = std::vector<bool>();
			}

			Split(unitsToCodes, v, 0, v.size(), 0.5);
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

		BitsVector MakeHead(const std::vector<unit_normalized>& unitsToNorm)
		{
			std::vector<bool> head;
			//head.reserve();
			uint8_t dictSize = unitsToNorm.size();
			uint8_t codeSize = 0;
			PushBytes(head, dictSize);//size of a dict
			for (const auto& uc : unitsToNorm)
			{
				PushBytes(head, uc.first);
				PushBytes(head, uc.second);
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
			auto codes_norm = f.GetScaled();
			BitsVector msg = MakeHead(codes_norm);
			PushBytes(msg, f.GetTotal());
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