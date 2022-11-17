#pragma once
#include "compression.h"
#include <algorithm>

namespace Compression
{

	const unit SLIDING_WINDOW_SIZE = 0xFF;
	const unit REF_UNIT = 0;

	using InputByteStream = std::istream;
	using OutputByteStream = std::ostream;


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

		void ProcessRef(BytesVector& output)
		{
			for (unit i = 0; i < _ref.len; i++)
			{
				size_t end = _window.size() - 1;
				unit u = _window[end - _ref.offset];
				PushWindow(u);
				output.push_back(u);
			}
		}

		bool Decode(unit u, BytesVector& out)
		{
			if (u == REF_UNIT) {
				if (_isRef) //ref symbol twice
				{
					out.push_back(REF_UNIT);
					PushWindow(REF_UNIT);
					_isRef = false;
					return true;
				}
				_isRef = true;
				return false;
			}
			if (_isRef)
			{
				if (_ref.len == 0)
				{
					_ref.len = u;
					return false;
				}
				_ref.offset = u;
				this->ProcessRef(out);
				_isRef = false;
				_ref.len = 0;
				return true;
			}
			PushWindow(u);
			out.push_back(u);
			return true;
		}

		bool Encode(unit u, BytesVector& out)
		{


			unit prevLength = _matchLength;
			unit prevPos = _matchPos;
			_msg.push_back(u);

			if (Match())
			{
				if (_msg.size() == _size)
				{
					out = End();
					_msg.clear();
				}
			}
			else
			{
				_msg.pop_back();
				if (_msg.size() > 2)
				{
					//put ref:
					out.push_back(REF_UNIT);
					out.push_back(prevLength);
					out.push_back(prevPos);
				}
				else
				{
					for (unit uo : _msg)
					{
						if (uo == REF_UNIT)
							out.push_back(REF_UNIT);
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
					_matchPos = _window.size() - i;
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
			if (_matchLength != 0)
			{
				out.push_back(REF_UNIT);
				out.push_back(_matchLength);
				out.push_back(_matchPos);
			}
			else
			{
				for (unit uo : _msg)
				{
					if (uo == REF_UNIT)
						out.push_back(REF_UNIT);
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
		unit _matchPos = 0;
		unit _matchLength = 0;
		bool _isRef = false;
		struct Ref {
			unit len = 0;
			unit offset = 0;
		} _ref;
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
				window.Encode(u, cache);
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
			BytesVector res;
			BytesVector cache;
			SlidingWindow w;
			for (size_t i = 0; i < input.size(); i++)
			{
				if (w.Decode(input[i], cache))
				{
					res.insert(res.end(), cache.begin(), cache.end());
					cache.clear();
				}
			}

			return res;

		}

	}


}