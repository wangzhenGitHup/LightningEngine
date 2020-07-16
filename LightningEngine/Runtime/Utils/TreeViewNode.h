#pragma once

#include <vector>

namespace LightningEngine
{
	class TreeViewNode
	{
	public:
		TreeViewNode();
		int AddChild(TreeViewNode* node, int pos = -1);
		void InsertBefore(TreeViewNode* node);
		void InsertAfter(TreeViewNode* node);
		void RemoveChild(TreeViewNode* node);
		void RemoveChildAt(unsigned int pos);
		TreeViewNode* GetLeftSibling();
		TreeViewNode* GetRightSibling();
		TreeViewNode* GetChild(int pos);
		TreeViewNode* GetParent();
		void SetParent(TreeViewNode* node);
		int GetChildPos(TreeViewNode* node);
		std::vector<TreeViewNode*>& GetChildren() { return children_; }

	protected:
		TreeViewNode* parent_;
		TreeViewNode* leftSibling_;
		TreeViewNode* rightSibling_;
		std::vector<TreeViewNode*> children_;
	};
}