#ifndef DYNAMIC_HPP_INCLUDED
#define DYNAMIC_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/interprocess/detail/atomic.hpp>

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
//			typename T,
//			template <typename> class Holder
//		 >
//struct dynamic_ptr
//{
//
//	dynamic_ptr() {}
//
//
//	//dynamic_ptr(Holder<T>* holder) 
//	//	: holder_(holder)
//	//{}
//
//	////int* operator->() const
//	//T* operator->() const
//	//{
//	//	return holder_->ptr_.pointee_;
//	//}
//
//	//Holder<T>* holder_;
//
//	//dynamic_ptr(T* t, H<T>* holder) : holder_(holder)
//	//{}
//
//	//T* operator->() const
//	//{
//	//	return holder_->ptr_.pointee_;
//	//}
//
//	//H<T>* holder_;
//};

namespace boost
{

template<class T> class dynamic_ptr;

namespace detail
{

template<class T> struct dynamic_ptr_traits
{
    typedef T & reference;
};

template<> struct dynamic_ptr_traits<void>
{
    typedef void reference;
};

#if !defined(BOOST_NO_CV_VOID_SPECIALIZATIONS)

template<> struct dynamic_ptr_traits<void const>
{
    typedef void reference;
};

template<> struct dynamic_ptr_traits<void volatile>
{
    typedef void reference;
};

template<> struct dynamic_ptr_traits<void const volatile>
{
    typedef void reference;
};

#endif


} // namespace detail


//
//  dynamic_ptr
//
//  
//

template <typename Holder> class dynamic_ptr
{
private:

    // Borland 5.5.1 specific workaround
    typedef dynamic_ptr<Holder> this_type;

public:

	typedef typename Holder::element_type element_type;
    typedef typename Holder::value_type value_type;
    typedef typename Holder::pointer pointer;
    typedef typename Holder::reference reference;

	//TODO: move semantics

	dynamic_ptr() 
		: holder_(0)
	{}

	//explicit dynamic_ptr(const Holder* holder) 
	explicit dynamic_ptr(Holder* holder) 
		: holder_(holder)
	{}

	dynamic_ptr(const dynamic_ptr<Holder>& other)
		: holder_(other.holder_)
	{
	}

    //dynamic_ptr<Holder>& operator=( dynamic_ptr<Holder> const & r ) // never throws
    dynamic_ptr<Holder>& operator=( const dynamic_ptr<Holder>& r ) // never throws
    {
        this_type(r).swap(*this);
		return *this;
    }

	

    //void reset() // never throws
    //{
    //    this_type().swap(*this);
    //}

    void swap(dynamic_ptr<Holder> & other) // never throws
    {
        std::swap(holder_, other.holder_);
    }

	typename Holder::pointer operator->() const
	{
		return holder_->ptr_.get();
	}

	Holder* holder_;
};

} // namespace boost


template <typename T>
class changer
{
public:
    typedef T element_type;
    typedef T value_type;
    typedef T * pointer;
    typedef typename boost::detail::dynamic_ptr_traits<T>::reference reference;

	friend class boost::dynamic_ptr< changer<T> >;

	void load()
	{
		ptr_.reset(new T);
		ptr_->data_ = 10;
	}

	void reload()
	{
		//change_ptr<T> ptrTemp(new T);
		//ptrTemp->data_ = 20;
		//ptr_.reset(ptrTemp.pointee_);
		//ptr_ = ptrTemp; //TODO: atomic swap

		T* temp =  new T;
		temp->data_ = 20;



		boost::interprocess::detail::atomic_cas32( ptr_.get(), temp, ptr_.get());

		////ptr_.reset(ptrTemp.pointee_);
		//ptr_.reset( temp ); //TODO: atomic swap
		////ptr_ = ptrTemp; //TODO: atomic swap
	}

	boost::dynamic_ptr< changer<T> > get()
	{
		return boost::dynamic_ptr< changer<T> >(this);
	}

	//boost::shared_ptr<T> get()
	//{
	//	return ptr_;
	//}

private:
	//change_ptr<T> ptr_; //TODO: cambiar por un shared_ptr modificado...
	boost::shared_ptr<T> ptr_;
};

#endif //DYNAMIC_HPP_INCLUDED
