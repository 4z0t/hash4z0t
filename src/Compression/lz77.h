#pragma once
#include "compression.h"


namespace Compression
{

	const unit SLIDING_WINDOW_SIZE = 256;


	class  SlidingWindow
	{
	public:
		SlidingWindow(unit size);
		~SlidingWindow();

	private:
		const unit _size = SLIDING_WINDOW_SIZE;
		unit _pos = 0;
	};

	SlidingWindow::SlidingWindow(unit size) :_size(size)
	{

	}



	SlidingWindow::~SlidingWindow()
	{
	}


	namespace LZ77
	{

		BitsVector	Compress(const BytesVector& input)
		{

		}




		BytesVector Decompress(const BitsVector& input)
		{

		}

	}


}