#pragma once

namespace LightningEngine
{
	class TTree
	{
	public:
		TTree();
		virtual ~TTree();
		bool AppendChild(TTree* node);
		bool InsertBefore(TTree* before);
		bool InsertAfter(TTree* after);
		void RemoveChild(TTree* node);
		bool IsParent(TTree* node);
		void Clean();

		template<class T>
		T* LeftSibling()
		{
			return (T*)leftSibling_;
		}

		template<class T>
		T* RightSibling()
		{
			return (T*)rightSibling_;
		}

		template<class T>
		T* Child()
		{
			return (T*)child_;
		}

		template<class T>
		T* Parent()
		{
			return (T*)parent_;
		}

	public:
		TTree* parent_;
		TTree* child_;
		TTree* leftSibling_;
		TTree* rightSibling_;
		TTree* lastChild_;
	};
}