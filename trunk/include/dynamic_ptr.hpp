#ifndef DYNAMIC_HPP_
#define DYNAMIC_HPP_

struct custom_class
{
	int data_;
};

template <typename T>
struct change_ptr
{
	change_ptr() : pointee_(0)
	{
	}

	change_ptr(T* t) : pointee_(t)
	{
	}

	void reset(T* t)
	{
		pointee_ = t;
	}

	T* operator->() const
	{
		return pointee_;
	}

	T* pointee_;
};

template <typename T>
struct changer;

template <typename T, typename H>
struct dynamic_ptr
{
	dynamic_ptr(T* t, H* holder) : holder_(holder)
	{}

	T* operator->() const
	{
		return holder_->ptr_.pointee_;
	}

	H* holder_;
};

template <typename T>
struct changer
{
	void load()
	{
		ptr_.reset(new T);
		ptr_->data_ = 10;
	}

	void reload()
	{
		change_ptr<T> ptrTemp(new T);
		ptrTemp->data_ = 20;
		ptr_.reset(ptrTemp.pointee_);
		ptr_ = ptrTemp; //TODO: atomic swap
	}

	//change_ptr<T> get()
	dynamic_ptr<T, changer<T> > get()
	{
		return dynamic_ptr<T, changer<T> >(ptr_.pointee_, this);
	}

	change_ptr<T> ptr_; //TODO: cambiar por un shared_ptr modificado...
};

#endif //DYNAMIC_HPP_
