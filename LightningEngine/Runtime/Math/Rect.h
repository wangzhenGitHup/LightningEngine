#pragma once

#include "Vector2.h"

namespace LightningEngine
{
	template<class T>
	class Rect
	{
	public:
		Rect(T x, T y, T width, T height) : left_(x), bottom_(y), width_(width), height_(height)
		{}

		Rect() : left_(0), bottom_(0), width_(0), height_(0)
		{}

		inline T GetRight() { return left_ + width_; }
		inline T GetBottom() { return bottom_ + height_; }
		inline bool Contains(T x, T y)
		{
			if (x > left_ && x < (left_ + width_) &&
				y > bottom_ && y < (bottom_ + height_))
			{
				return true;
			}

			return false;
		}

		void Set(T x, T y, T width, T height)
		{
			left_ = x;
			bottom_ = y;
			width_ = width;
			height_ = height;
		}

		bool IsOverlappedWidth(const Rect& other)
		{
			if (left_ > (other.left_ + other.width_) ||
				other.left_ > (left_ + width_))
			{
				return false;
			}

			if ((bottom_ + height_) < other.bottom_ ||
				(other.bottom_ + other.height_) < bottom_)
			{
				return false;
			}

			return true;
		}

	public:
		T left_;
		T bottom_;
		T width_;
		T height_;
	};
}