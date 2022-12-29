#pragma once
#include "compression.h"
#include <algorithm>

namespace Compression
{

	const unit SLIDING_WINDOW_SIZE = 0xFF;

	using InputByteStream = std::istream;
	using OutputByteStream = std::ostream;





	namespace LZ77
	{

		class  SlidingWindow
		{
		public:
			struct Ref {
				unit len = 0;
				unit offset = 0;

			};

			SlidingWindow(unit size, unit ref) : _size(size), _refUnit(ref)
			{
				_window.reserve(_size);
			}

			SlidingWindow(unit size) : SlidingWindow(size, 0) {	}

			SlidingWindow() : SlidingWindow(SLIDING_WINDOW_SIZE) {}

			bool Decode(unit u, BytesVector& out)
			{
				if (_isRef && u == 0) //ref symbol and 0
				{
					out.push_back(_refUnit);
					PushWindow(_refUnit);
					_isRef = false;
					return true;
				}
				if (u == _refUnit && !_isRef) {

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
					UnpackRef(out);
					_isRef = false;
					return true;
				}
				PushWindow(u);
				out.push_back(u);
				return true;
			}

			bool Encode(unit u, BytesVector& out)
			{

				Ref prev = _ref;
				_msg.push_back(u);
				PushWindow(u);

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
					if (_msg.size() > 3)
					{
						//put ref:
						PushRef(out, prev);
					}
					else
					{
						for (unit uo : _msg)
						{
							out.push_back(uo);
							if (uo == _refUnit)
								out.push_back(0);
						}

					}
					_msg.clear();
					_msg.push_back(u);
				}

				return out.size() != 0;
			}

			BytesVector End()
			{
				BytesVector out;
				//std::cout << (int)_ref.len << std::endl;
				if (_ref.len != 0)
				{
					PushRef(out, _ref);
				}
				else
				{
					for (unit uo : _msg)
					{

						out.push_back(uo);
						if (uo == _refUnit)
							out.push_back(0);
					}
				}
				return out;
			}

			void SetRefUnit(unit ref)
			{
				_refUnit = ref;
			}

		private:
			void UnpackRef(BytesVector& output)
			{

				while (_ref.len != 0)
				{
					size_t end = _window.size();
					unit u = _window[end - _ref.offset];
					PushWindow(u);
					output.push_back(u);
					_ref.len--;
				}
			}

			bool Match()
			{
				_ref.len = 0;
				_ref.offset = 0;

				unit length = 0;
				unit pos = 0;
				for (size_t i = 0; i < _window.size(); i++)
				{
					if (_msg.size() == length)//whole msg fits
					{
						_ref.len = length;
						_ref.offset = _window.size() - i;
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
				return _ref.len != 0;
			}

			void PushWindow(unit u)
			{
				if (_window.size() == _size)
				{
					ShiftLeft(_window, 1);
				}
				_window.push_back(u);
			}

			void PushRef(BytesVector& out, Ref ref)
			{
				out.push_back(_refUnit);
				out.push_back(ref.len);
				out.push_back(ref.offset);
			}

			BytesVector _msg;							//message being processed
			BytesVector _window;						//sliding window itself
			const unit _size = SLIDING_WINDOW_SIZE;		//size
			bool _isRef = false;
			Ref _ref;
			unit _refUnit;

		};


		BytesVector Compress(const BytesVector& input)
		{
			BytesVector res;
			BytesVector cache;
			cache.reserve(3);
			SlidingWindow window;
			for (unit u : input)
			{
				if (window.Encode(u, cache))
				{
					res.insert(res.end(), cache.begin(), cache.end());
					cache.clear();
				}
			}
			auto end = window.End();
			res.insert(res.end(), end.begin(), end.end());
			return res;
		}




		BytesVector Decompress(const BytesVector& input, size_t len)
		{
			BytesVector res;
			BytesVector cache;
			SlidingWindow window;
			size_t i = 0;
			for (size_t j = 0; j < len; i++)
			{
				unit u = input[i];
				if (window.Decode(u, cache))
				{
					j += cache.size();
					res.insert(res.end(), cache.begin(), cache.end());
					cache.clear();
				}
			}

			return res;

		}

	}


}