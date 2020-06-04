#pragma once

namespace LightningEngine
{
	/*
		µ¥Á´±í
	*/
	class LinkedList
	{
	public:
		LinkedList() : next_(nullptr) {}

		virtual void PushBack(LinkedList* pnode)
		{
			if (next_ == nullptr)
			{
				next_ = pnode;
			}
			else
			{
				if (next_ == pnode)
				{
					return;
				}

				Next<LinkedList>()->PushBack(pnode);
			}
		}

		virtual void Remove(LinkedList* pnode)
		{
			if (next_ != nullptr)
			{
				if (pnode == next_)
				{
					next_ = pnode->Next<LinkedList>();
					pnode->next_ = nullptr;
				}
				else
				{
					Next<LinkedList>()->Remove(pnode);
				}
			}
		}

		LinkedList* GetNext() { return next_; }

	//protected:
		template<class T>
		T* Next()
		{
			return (T*)next_;
		}

	//protected:
		LinkedList* next_;

	};

	
	/*
		Ë«Á´±í
	*/
	class DoubleLinkedList
	{
	public:
		DoubleLinkedList() : prev_(nullptr), next_(nullptr){}
		virtual ~DoubleLinkedList()
		{
			ClearList();
		}

		void operator<<(int gap)
		{
			DoubleLinkedList* pNodeToShift = this;
			DoubleLinkedList* pTargetPos = Next<DoubleLinkedList>();
			while (--gap)
			{
				pTargetPos = pTargetPos->Next<DoubleLinkedList>();
			}

			pNodeToShift->InsertAfter(pTargetPos);
		}

		void Append(DoubleLinkedList* pnode)
		{
			if (next_ != nullptr)
			{
				next_->Append(pnode);
			}
			else
			{
				next_ = pnode;
				pnode->prev_ = this;
			}
		}

		void InsertBefore(DoubleLinkedList* pnode)
		{
			ClearList();
			if (pnode->prev_ != nullptr)
			{
				pnode->prev_->next_ = this;
			}

			prev_ = pnode->prev_;
			pnode->prev_ = this;
			next_ = pnode;
		}

		void InsertAfter(DoubleLinkedList* pnode)
		{
			ClearList();
			if (pnode->next_ != nullptr)
			{
				pnode->next_->prev_ = this;
			}

			next_ = pnode->next_;
			pnode->next_ = this;
			prev_ = pnode;
		}

		void ClearList()
		{
			if (prev_ != nullptr)
			{
				prev_->next_ = next_;
			}

			if (next_ != nullptr)
			{
				next_->prev_ = prev_;
			}

			prev_ = nullptr;
			next_ = nullptr;
		}

		DoubleLinkedList* GetNext() { return next_; }
		DoubleLinkedList* GetPrev() { return prev_; }
		void ClearNext() { next_ = nullptr; }

	//private:
		template<class T>
		T* Next()
		{
			return (T*)next_;
		}

		template<class T>
		T* Prev()
		{
			return (T*)prev_;
		}
	private:
		DoubleLinkedList* prev_;
		DoubleLinkedList* next_;
	};
}