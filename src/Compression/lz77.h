#pragma once
#include "compression.h"
#include <algorithm>

namespace Compression
{

	const unit SLIDING_WINDOW_SIZE = 0xFF;


	template<typename T>
	void ShiftLeft(std::vector<T>& vec, size_t shift)
	{
		for (size_t i = 0; i < vec.size() - shift; i++)
		{
			vec[i] = vec[shift + i];
		}
		vec.resize(vec.size() - shift);
	}


	class  SlidingWindow
	{
	public:
		SlidingWindow();
		SlidingWindow(unit size);
		~SlidingWindow();


		bool Next(unit u, BytesVector& out)
		{
			if (_window.empty())
			{
				if (u == 0)
					out.push_back(u);
				out.push_back(u);

				_window.push_back(u);
				return true;
			}



			unit prevLength = _matchLength;
			unit prevPos = _matchPos;
			_msg.push_back(u);

			if (Match())
			{

			}
			else
			{
				_msg.pop_back();
				if (_msg.size() > 2)
				{
					//put ref:
					out.push_back(0);
					out.push_back(prevPos);
					out.push_back(prevLength);
				}
				else
				{
					for (unit uo : _msg)
					{
						if (uo == 0)
							out.push_back(uo);
						out.push_back(uo);
					}

				}
				_msg.clear();
				_msg.push_back(u);
			}
			PushWindow(u);







			return false;
		}


		bool Match()
		{
			_matchLength = 0;
			_matchPos = 0;

			unit length = 0;
			unit pos = 0;
			for (unit i = 0; i < _window.size(); i++)
			{
				if (_msg.size() == length)//whole msg fits
				{
					_matchLength = length;
					_matchPos = i - length;
					break;
				}

				if (_msg[length] == _window[i])//continue if match
				{
					length++;
				}
				else
				{
					//if (_matchLength < length + 1)//find the largest
					//{
					//	_matchLength = length + 1;
					//	_matchPos = i - length;
					//}
					//reset
					i -= length;
					length = 0;
				}
			}
			//return if we found anything
			return _matchLength != 0;
		}



		BytesVector End()
		{
			BytesVector out;
			if (Match())
			{
				out.push_back(0);
				out.push_back(_matchPos);
				out.push_back(_matchLength);
			}
			else
			{
				for (unit uo : _msg)
				{
					if (uo == 0)
						out.push_back(uo);
					out.push_back(uo);
				}
			}
			return out;
		}


		void PushWindow(unit u)
		{
			if (_window.size() == _size)
			{
				ShiftLeft(_window, 1);
			}
			_window.push_back(u);
		}

	private:

		BytesVector _msg;							//message being processed
		BytesVector _window;						//sliding window itself
		const unit _size = SLIDING_WINDOW_SIZE;		//size
		unit _pos = 0;								//cur window pos
		unit _matchPos;
		unit _matchLength;
	};

	SlidingWindow::SlidingWindow(unit size) : _size(size)
	{
		_window.reserve(_size);
	}

	SlidingWindow::SlidingWindow() : SlidingWindow(SLIDING_WINDOW_SIZE)
	{
	}

	SlidingWindow::~SlidingWindow()
	{

	}


	namespace LZ77
	{

		BytesVector Compress(const BytesVector& input)
		{
			BytesVector res;
			BytesVector cache;
			cache.reserve(3);
			SlidingWindow window;
			for (unit u : input)
			{
				window.Next(u, cache);
				if (cache.size() != 0)
				{
					res.insert(res.end(), cache.begin(), cache.end());
					cache.clear();
				}
			}
			auto end = window.End();
			res.insert(res.end(), end.begin(), end.end());
			return res;
		}




		BytesVector Decompress(const BytesVector& input)
		{
			return BytesVector();
		}

	}


}