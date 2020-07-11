#include "TTree.h"

namespace LightningEngine
{
	TTree::TTree() : parent_(nullptr),
		child_(nullptr),
		leftSibling_(nullptr),
		rightSibling_(nullptr),
		lastChild_(nullptr)
	{
	}

	TTree::~TTree()
	{
	}

	bool TTree::AppendChild(TTree * node)
	{
		if (IsParent(node))
		{
			return false;
		}
		
		node->Clean();
		TTree* child = child_;
		if (nullptr == child_)
		{
			child_ = node;
			node->parent_ = this;
		}
		else
		{
			while (child->rightSibling_ != nullptr)
			{
				child = child->rightSibling_;
			}

			child->rightSibling_ = node;
			node->parent_ = this;
			node->leftSibling_ = child;
		}

		lastChild_ = node;


		return true;
	}

	bool TTree::InsertBefore(TTree * before)
	{
		if (before->IsParent(this))
		{
			return false;
		}

		Clean();

		if (before->parent_ != nullptr)
		{
			if (before->leftSibling_ == nullptr)
			{
				before->parent_->child_ = this;
			}
			else
			{
				leftSibling_ = before->leftSibling_;
				leftSibling_->rightSibling_ = this;
			}

			parent_ = before->parent_;
			rightSibling_ = before;
			before->leftSibling_ = this;
		}
		return true;
	}

	bool TTree::InsertAfter(TTree * after)
	{
		if (after->IsParent(this))
		{
			return false;
		}

		Clean();

		parent_ = after->parent_;
		if (after->rightSibling_ != nullptr)
		{
			rightSibling_ = after->rightSibling_;
			rightSibling_->leftSibling_ = this;
		}
		else
		{
			if (parent_ != nullptr)
			{
				parent_->lastChild_ = this;
			}
		}

		leftSibling_ = after;
		after->rightSibling_ = this;
		return true;
	}

	void TTree::RemoveChild(TTree * node)
	{
		if (node == child_)
		{
			if (node->rightSibling_ != nullptr)
			{
				child_ = node->rightSibling_;
				node->rightSibling_->leftSibling_ = nullptr;
			}
			else
			{
				child_ = nullptr;
				lastChild_ = nullptr;
			}
		}
		else
		{
			if (node->rightSibling_ != nullptr)
			{
				node->rightSibling_->leftSibling_ = node->leftSibling_;
				node->leftSibling_->rightSibling_ = node->rightSibling_;
			}
			else
			{
				node->leftSibling_->rightSibling_ = nullptr;
				lastChild_ = node->leftSibling_;
			}
		}

		node->leftSibling_ = nullptr;
		node->rightSibling_ = nullptr;
		node->parent_ = nullptr;
	}

	bool TTree::IsParent(TTree * node)
	{
		bool bParent = false;
		if (parent_ != nullptr)
		{
			if (node == parent_)
			{
				bParent = true;
			}

			if (bParent)
			{
				return true;
			}

			bParent = parent_->IsParent(node);
		}

		return bParent;
	}

	void TTree::Clean()
	{
		if (parent_ != nullptr)
		{
			parent_->RemoveChild(this);
		}
		else
		{
			if (leftSibling_ != nullptr)
			{
				if (rightSibling_ != nullptr)
				{
					leftSibling_->rightSibling_ = rightSibling_;
					rightSibling_->leftSibling_ = leftSibling_;
				}
				else
				{
					leftSibling_->rightSibling_ = nullptr;
				}

				leftSibling_ = nullptr;
				rightSibling_ = nullptr;
			}
		}
	}
}