#include "Color.h"

namespace LightningEngine
{
	Color4B::Color4B() : r_(255), g_(255), b_(255), a_(255)
	{}

	Color4B::Color4B(const char* hexStr)
	{
		char digits[3];
		char* error;

		if (strlen(hexStr) != 8)
		{
			r_ = 255;
			g_ = 255;
			b_ = 255;
			a_ = 255;
		}

		digits[0] = *hexStr;
		digits[1] = *(hexStr + 1);
		digits[2] = '\0';
		r_ = (UInt8)strtoul(digits, &error, 16);

		digits[0] = *(hexStr + 2);
		digits[1] = *(hexStr + 3);
		digits[2] = '\0';
		g_ = (UInt8)strtoul(digits, &error, 16);

		digits[0] = *(hexStr + 4);
		digits[1] = *(hexStr + 5);
		digits[2] = '\0';
		b_ = (UInt8)strtoul(digits, &error, 16);

		digits[0] = *(hexStr + 6);
		digits[1] = *(hexStr + 7);
		digits[2] = '\0';
		a_ = (UInt8)strtoul(digits, &error, 16);
	}

	const Color4B Color4B::GREEN = Color4B(0, 255, 0, 255);
	const Color4B Color4B::WHITE = Color4B(255, 255, 255, 255);
	const Color4B Color4B::GRAY = Color4B(41, 41, 41, 255);
	const Color4B Color4B::BLACK = Color4B(0, 0, 0, 255);

	bool Color4B::operator!=(const Color4B& clr)const
	{
		return !(r_ == clr.r_ && g_ == clr.g_ && b_ == clr.b_ && a_ == clr.a_);
	}

	Color4B Lerp(Color4B from, Color4B to, float percent)
	{
		return Color4B(from * percent + to * (1.0f - percent));
	}
}
