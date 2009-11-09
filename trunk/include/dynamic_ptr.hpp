#ifndef DYNAMIC_HPP_
#define DYNAMIC_HPP_

struct custom_class
{
	int data_;
};

//IDEM shared_ptr
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

//template <typename T, template <typename T> class H>
//template <typename T, typename H>

//template <
//			class T, 
//			template <class Created> class H
//		 >

//template <
//			template <typename T> class H
//		 >


template <
			typename T,
			template <typename T> class H
		 >
struct dynamic_ptr
{

	dynamic_ptr() {}


	dynamic_ptr(H<T>* holder) : holder_(holder)
	{}

	//int* operator->() const
	T* operator->() const
	{
		return holder_->ptr_.pointee_;
	}

	H<T>* holder_;

	//dynamic_ptr(T* t, H<T>* holder) : holder_(holder)
	//{}

	//T* operator->() const
	//{
	//	return holder_->ptr_.pointee_;
	//}

	//H<T>* holder_;
};

template <typename T>
T make_pepe(T pepe)
{
	return pepe;
}

template <
			typename T,
			template <typename T> class H
		 >
dynamic_ptr< T, H > make_dynamic_ptr(T t, H<T> h)
{
	return dynamic_ptr<T, H>(t, h);
}



template <typename T>
class changer
{
public:
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

	//dynamic_ptr<T, changer<T> > get()
	//dynamic_ptr<T, changer<T> > get()
	dynamic_ptr< T, changer<T> > get()
	{
		//return dynamic_ptr<T, changer<T> >(ptr_.pointee_, this);
		//return dynamic_ptr<T, changer>(ptr_.pointee_, this);
		//return dynamic_ptr< T, changer >( this );
		//return dynamic_ptr< T, changer >();
	}



	change_ptr<T> ptr_; //TODO: cambiar por un shared_ptr modificado...
};

#endif //DYNAMIC_HPP_
