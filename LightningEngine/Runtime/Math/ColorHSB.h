#pragma once
#include "RuntimePrefix.h"
#include "Color.h"

namespace LightningEngine
{
	class ColorHSB
	{
	public:
		ColorHSB(UInt8 r, UInt8 g, UInt8 b);
		ColorHSB(Color4B& clr);
		ColorHSB();
		void ToRGB(Color4B& clr);

		UInt16 h_, s_, b_;
	
	};
}