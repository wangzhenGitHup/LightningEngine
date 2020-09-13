#pragma once

#include "Math/Rect.h"
#include "Math/Vector4.h"

namespace LightningEngine
{
	class TextureRectangleNode
	{
	public:
		TextureRectangleNode(bool bLeaf);
		TextureRectangleNode* AddTexture(int& width, int& height);

	public:
		TextureRectangleNode * child_[2];
		Rect<int> rect_;
		Vector4 texCoord_;
		bool bLeaf_;
		bool bFilled_;
		bool bRotated_;

	};
}