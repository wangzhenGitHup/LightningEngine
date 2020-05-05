#pragma once

#include <algorithm>
#include "RuntimePrefix.h"

namespace LightningEngine
{
	struct Color4B
	{
		UInt8 r_;
		UInt8 g_;
		UInt8 b_;
		UInt8 a_;

		Color4B();
		Color4B(UInt8 r, UInt8 g, UInt8 b, UInt8 a = 255)
		{
			r_ = r; g_ = g; b_ = b; a_ = a;
		}

		Color4B(const char* hexStr);
		bool operator==(const Color4B& clr)const
		{
			return (r_ == clr.r_ && g_ == clr.g_ && b_ == clr.b_ && a_ == clr.a_);
		}

		bool operator!=(const Color4B& clr)const;
		void operator=(const Color4B& clr)
		{
			r_ = clr.r_;
			g_ = clr.g_;
			b_ = clr.b_;
			a_ = clr.a_;
		}

		Color4B operator+(const Color4B& clr)
		{
			return Color4B(r_ + clr.r_, g_ + clr.g_, b_ + clr.b_, a_ + clr.a_);
		}

		Color4B operator*(float val)
		{
			r_ = UInt8(r_ * val);
			g_ = UInt8(g_ * val);
			b_ = UInt8(b_ * val);
			a_ = UInt8(a_ * val);
			return *this;
		}

		void Set(UInt8 r, UInt8 g, UInt8 b, UInt8 a = 255)
		{
			r_ = r;
			g_ = g;
			b_ = b;
			a_ = a;
		}

		static const Color4B WHITE;
		static const Color4B YELLOW;
		static const Color4B BLUE;
		static const Color4B GREEN;
		static const Color4B RED;
		static const Color4B MAGENTA;
		static const Color4B BLACK;
		static const Color4B ORANGE;
		static const Color4B GRAY;
	};

	Color4B Lerp(Color4B from, Color4B to, float percent);
}