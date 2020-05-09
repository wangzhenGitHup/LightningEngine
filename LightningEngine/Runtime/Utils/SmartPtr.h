#pragma once

template<class T>
class SmartPtr
{
public:
	SmartPtr(T* ptr = nullptr) : ptr_(ptr) {
		if (nullptr != ptr_)
		{
			ptr_->retain();
		}
	}

	~SmartPtr()
	{
		if (nullptr != ptr_)
		{
			ptr_->release();
		}
	}

	T* Release()
	{
		if (nullptr != ptr_)
		{
			ptr_->release();
		}

		return ptr_;
	}

	T* operator->() { return ptr_; }
	T* operator->()const { return ptr_; }
	T& operator*() { return *ptr_; }
	bool operator==(T* ptr) { return ptr_ == ptr; }
	bool operator!=(T* ptr) { return ptr_ != ptr; }
	bool operator==(SmartPtr<T>& s) { return ptr_ == s.ptr_; }
	void operator=(SmartPtr<T>& s)
	{
		if (nullptr != s.ptr_)
		{
			s.ptr_->retain();
		}

		if (nullptr != ptr_)
		{
			ptr_->release();
		}

		ptr_ = s.ptr_;
	}

	void operator=(T* ptr)
	{
		if (nullptr != ptr)
		{
			ptr->retain();
		}

		if (nullptr != ptr_)
		{
			ptr_->release();
		}

		ptr_ = ptr;
	}

public:
	T* ptr_;
};