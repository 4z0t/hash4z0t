#include "../Chunk.h"
#include "../internalhash4z0t.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

namespace Compressions
{
	using unit = std::uint8_t;
	using std::size_t;
	using std::abs;

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
			std::vector<unit_count> unitCount(_data.begin(), _data.end());
			std::transform(
				unitCount.begin(),
				unitCount.end(),
				std::back_inserter(this->_data),
				[total](const unit_count& uc)
				{
					return unit_frequency(uc.first, (double)uc.second / total);
				}
			);
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
		size_t _total;
	};



	namespace ShannonFano
	{

		void Split(std::unordered_map<unit, Code>& unitsToCodes, const std::vector<unit_frequency>& frequency, size_t start, size_t end, double threshold)
		{
			switch (end - start)
			{
			case 2:
				unitsToCodes[frequency[start].first].push_back(false);
				unitsToCodes[frequency[start + 1].first].push_back(true);
			case 1:
			case 0:
				return;
			default:
				break;
			}
			size_t middle = start;
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
							//current one is edge
							unitsToCodes[key].push_back(true);
							middle = i;
						}
						else
						{
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
			Split(unitsToCodes, frequency, start, middle, threshold / 2);
			Split(unitsToCodes, frequency, middle, end, threshold / 2);
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

			for (auto& p : v)
			{

				std::cout << p.first << "\t[" << (int)(p.first) << "]\t";
				for (auto& bit : unitsToCodes[p.first])
				{
					std::cout << bit;
				}
				std::cout << "\n";
			}
			return unitsToCodes;
		}


		[[nodiscard]]
		Data Compress(const Data& input)
		{
			FrequencyTable f(input);
			std::unordered_map<unit, Code> unitsToCodes = FrequencyToCodes(f);



			return input;
		}

		[[nodiscard]]
		Data Decompress(const Data& input)
		{
			return input;
		}





	}


}