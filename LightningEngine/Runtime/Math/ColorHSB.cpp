#include "ColorHSB.h"
#include "Float.h"

namespace LightningEngine
{
	ColorHSB::ColorHSB(UInt8 r, UInt8 g, UInt8 b)
	{
		unsigned short max, min, delta;
		short temp;
		max = Max(r, g, b);
		min = Min(r, g, b);
		delta = max - min;

		if (max == 0)
		{
			s_ = h_ = b = 0;
			return;
		}

		b_ = (unsigned short)((double)max / 255.0f * 100.0f);
		s_ = (unsigned short)(((double)delta / max) * 100.0f);

		if (r == max)
		{
			temp = (short)(60 * ((g - b) / (double)delta));
		}
		else if (g == max)
		{
			temp = (short)(60 * (2.0f + (b - r) / (double)delta));
		}
		else
		{
			temp = (short)(60 * (4.0f + (r - g) / (double)delta));
		}

		if (temp < 0)
		{
			h_ = temp + 360;
		}
		else
		{
			h_ = temp;
		}
	}

	ColorHSB::ColorHSB() : h_(0), s_(0), b_(0){}

	ColorHSB::ColorHSB(Color4B& clr)
	{
		UInt8 clrR = clr.r_, clrG = clr.g_, clrB = clr.b_;
		unsigned short max, min, delta;
		short temp;

		max = Max(clrR, clrG, clrB);
		min = Min(clrR, clrG, clrB);
		delta = max - min;

		if (0 == max)
		{
			s_ = h_ = b_ = 0;
			return;
		}

		b_ = (unsigned short)((double)max / 255.0f * 100.0f);
		s_ = (unsigned short)((double)delta / max) * 100.0f;

		if (max == clrR)
		{
			temp = (short)(60 * ((clrG - clrB) / (double)delta));
		}
		else if (max == clrG)
		{
			temp = (short)(60 * (2.0f + (clrB - clrR) / (double)delta));
		}
		else
		{
			temp = (short)(60 * (4.0f + (clrR - clrB) / (double)delta));
		}

		if (temp < 0)
		{
			h_ = temp + 360;
		}
		else
		{
			h_ = temp;
		}
	}

	void ColorHSB::ToRGB(Color4B& clr)
	{
		int conv;
		double hue, sat, val;
		int base;

		hue = (float)h_ / 100.0f;
		sat = (float)s_ / 100.0f;
		val = (float)b_ / 100.0f;

		if (FLT_IS_ZERO((float)s_))
		{
			conv = (unsigned short)(255.0f * val);
			clr.r_ = clr.g_ = clr.b_ = conv;
			clr.a_ = 255;
			return;
		}

		base = (unsigned short)(255.0f * (1.0f - sat) * val);
		unsigned short flag = unsigned short((float)h_ / 60.0f);
		switch (flag)
		{
		case 0:
			clr.r_ = (unsigned short)(255.0f * val);
			clr.g_ = (unsigned short)((255.0f * val - base) * (h_ / 60.0f) + base);
			clr.b_ = base;
			break;

		case 1:
			clr.r_ = (unsigned short)((255.0f * val - base) * (1.0f - ((h_ % 60) / 60.0f)) + base);
			clr.g_ = (unsigned short)(255.0f * val);
			clr.b_ = base;
			break;

		case 2:
			clr.r_ = base;
			clr.g_ = (unsigned short)(255.0f * val);
			clr.b_ = (unsigned short)((255.0f * val - base) * ((h_ % 60) / 60.0f) + base);
			break;

		case 3:
			clr.r_ = base;
			clr.g_ = (unsigned short)(255.0f * val - base) * (1.0f - ((h_ % 60) / 60.0f) + base);
			clr.b_ = (unsigned short)(255.0f * val);
			break;

		case 4:
			clr.r_ = (unsigned short)((255.0f * val - base) * ((h_ % 60) / 60.0f) + base);
			clr.g_ = base;
			clr.b_ = (unsigned short)(255.0f * val);
			break;

		case 5:
			clr.r_ = (unsigned short)(255.0f * val);
			clr.g_ = base;
			clr.b_ = (unsigned short)((255.0f * val - base) * (1.0f - ((h_ % 60) / 60.0f)) + base);
			break;
		}
	}
}