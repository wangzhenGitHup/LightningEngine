#include "TextureRectangleNode.h"

namespace LightningEngine
{
	TextureRectangleNode::TextureRectangleNode(bool bLeaf) : 
		bLeaf_(bLeaf), bRotated_(false), bFilled_(false)
	{
	}

	TextureRectangleNode * TextureRectangleNode::AddTexture(int & width, int & height)
	{
		if (bLeaf_)
		{
			if (bFilled_)
			{
				return nullptr;
			}

			if (rect_.width_ < width ||
				rect_.height_ < height)
			{
				return nullptr;
			}
			else if (rect_.width_ == width && rect_.height_ == height)
			{
				bFilled_ = true;
				return this;
			}

			// split this to A -> B and find a rectangle in A for given image
			child_[0] = new TextureRectangleNode(true);
			child_[1] = new TextureRectangleNode(true);
			bLeaf_ = false;

			int dWidth = rect_.width_ - width;
			int dHeight = rect_.height_ - height;
			const int gap = 3;
			if (dWidth > dHeight)
			{
				((TextureRectangleNode*)child_[0])->rect_.Set(rect_.left_,
					rect_.bottom_, rect_.width_, height);
				((TextureRectangleNode*)child_[1])->rect_.Set(rect_.left_ + width + gap,
					rect_.bottom_, dHeight - gap, rect_.height_);
			}
			else
			{
				((TextureRectangleNode*)child_[0])->rect_.Set(rect_.left_,
					rect_.bottom_, rect_.width_, height);
				((TextureRectangleNode*)child_[1])->rect_.Set(rect_.left_,
					rect_.bottom_ + height + gap, rect_.width_, dHeight - gap);
			}

			return ((TextureRectangleNode*)child_[0])->AddTexture(width, height);
		}
		else
		{
			TextureRectangleNode* pNode = ((TextureRectangleNode*)child_[0])->AddTexture(width, height);
			if (nullptr != pNode)
			{
				return pNode;
			}

			return ((TextureRectangleNode*)child_[1])->AddTexture(width, height);
		}
	}
}