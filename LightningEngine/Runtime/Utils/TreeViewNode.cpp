#include "TreeViewNode.h"

namespace LightningEngine
{
	TreeViewNode::TreeViewNode() : parent_(nullptr),
		leftSibling_(nullptr),
		rightSibling_(nullptr)
	{
	}

	int TreeViewNode::AddChild(TreeViewNode * node, int pos)
	{
		int iRet = 1;
		if (-1 == pos)
		{
			children_.push_back(node);
			node->SetParent(this);
			iRet = (int)children_.size();
		}
		else
		{
			unsigned int cnt = children_.max_size();
			if (cnt - 1 >= pos)
			{
				TreeViewNode* pEnd = children_[cnt - 1];
				for (int i = cnt - 1; i > pos; i--)
				{
					children_[i] = children_[i - 1];
				}

				children_[pos] = node;
				children_.push_back(pEnd);
				if (0 != pos)
				{
					node->leftSibling_ = children_[pos - 1];
				}

				node->rightSibling_ = children_[pos + 1];
				children_[pos + 1]->leftSibling_ = node;
				node->SetParent(this);
				iRet = pos;
			}
			else
			{
				children_.push_back(node);
				node->SetParent(this);
				iRet = (int)children_.size();
			}
		}
		return iRet;
	}

	void TreeViewNode::InsertBefore(TreeViewNode * node)
	{
		if (nullptr != node->GetParent())
		{
			node->GetParent()->AddChild(this, node->GetParent()->GetChildPos(node));
		}
	}

	void TreeViewNode::InsertAfter(TreeViewNode * node)
	{
		if (nullptr != node->GetParent())
		{
			node->GetParent()->AddChild(this, node->GetParent()->GetChildPos(node) + 1);
		}
	}

	void TreeViewNode::RemoveChild(TreeViewNode * node)
	{
		for (std::vector<TreeViewNode*>::iterator iter = children_.begin();
			iter != children_.end(); ++iter)
		{
			if ((*iter) == node)
			{
				children_.erase(iter);
				break;
			}
		}
	}

	void TreeViewNode::RemoveChildAt(unsigned int pos)
	{
		if (children_.size() > pos)
		{
			RemoveChild(children_[pos]);
		}
	}

	TreeViewNode * TreeViewNode::GetLeftSibling()
	{
		return leftSibling_;
	}

	TreeViewNode * TreeViewNode::GetRightSibling()
	{
		return rightSibling_;
	}

	TreeViewNode * TreeViewNode::GetChild(int pos)
	{
		if (children_.size() > pos)
		{
			return children_[pos];
		}
		return nullptr;
	}

	TreeViewNode * TreeViewNode::GetParent()
	{
		return parent_;
	}

	void TreeViewNode::SetParent(TreeViewNode * node)
	{
		parent_ = node;
	}

	int TreeViewNode::GetChildPos(TreeViewNode * node)
	{
		int idx = 0;
		int cnt = (int)children_.size();
		for (; idx < cnt; idx++)
		{
			if (children_[idx] == node)
			{
				return idx;
			}
		}
		return -1;
	}
}