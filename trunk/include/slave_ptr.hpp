//TODO: nombre: Dependent Pointers

#ifndef SLAVE_HPP_INCLUDED
#define SLAVE_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
//#include <boost/interprocess/detail/atomic.hpp>
#include <boost/noncopyable.hpp>



namespace boost
{

template<class T> class slave_ptr;

namespace detail
{

template<class T> struct slave_ptr_traits
{
    typedef T & reference;
};

template<> struct slave_ptr_traits<void>
{
    typedef void reference;
};

#if !defined(BOOST_NO_CV_VOID_SPECIALIZATIONS)

template<> struct slave_ptr_traits<void const>
{
    typedef void reference;
};

template<> struct slave_ptr_traits<void volatile>
{
    typedef void reference;
};

template<> struct slave_ptr_traits<void const volatile>
{
    typedef void reference;
};

#endif


} // namespace detail



template <typename T>
class master_ptr;

//
//  slave_ptr
//
//  
//
template <typename T> 
class slave_ptr
{
private:

    // Borland 5.5.1 specific workaround
    typedef slave_ptr<T> this_type;

public:
    typedef T element_type;
    typedef T value_type;
    typedef T * pointer;
    typedef typename boost::detail::slave_ptr_traits<T>::reference reference;

	//TODO: move semantics

	slave_ptr() 
		: master_(0)
	{}

	explicit slave_ptr(master_ptr<T>* master) 
		: master_(master)
	{}

	slave_ptr(const slave_ptr<T>& other)
		: master_(other.master_)
	{
	}

  //  //slave_ptr<T>& operator=( slave_ptr<T> const & r ) // never throws
  //  slave_ptr<T>& operator=( const slave_ptr<T>& r ) // never throws
  //  {
  //      this_type(r).swap(*this);
		//return *this;
  //  }

	typename pointer operator->() const
	{
		//return master_->ptr_.get();
		return master_->get();
	}

    reference operator* () const // never throws
    {
        //BOOST_ASSERT(px != 0);
        return *master_->ptr_.get();
    }


	master_ptr<T>* master_;
};


// No debería ser copiable, ya que si se copia, 
// Es un shared_ptr con habilidad de hacer un atomic_swap
template <typename T>
class master_ptr : boost::noncopyable //: shared_ptr<T>
{
private:

    // Borland 5.5.1 specific workaround
    typedef master_ptr<T> this_type;

public:
    typedef T element_type;
    typedef T value_type;
    typedef T * pointer;
    typedef typename boost::detail::slave_ptr_traits<T>::reference reference;

	friend class boost::slave_ptr<T>;

	master_ptr() //: ptr_(0)
	{
	}

	master_ptr(T* ptr)
		: ptr_(ptr)
	{
	}

	slave_ptr<T> getSlave()
	{
		return slave_ptr<T>(this);
	}

	//TODO: ver si se puede hacer otro método que sea const.
	T* get()
	{
		return ptr_.get();
	}

	T* get() const
	{
		return ptr_.get();
	}

	typename pointer operator->() const
	{
		return ptr_.get();
	}

    void reset(T* ptr)
    {
		ptr_.reset(ptr);
    }

	bool atomic_exchange(master_ptr<T> * newPtr)
	{
		return atomic_compare_exchange( &ptr_, &ptr_, newPtr->ptr_ );
	}


private:
	boost::shared_ptr<T> ptr_;
};

} // namespace boost



#endif //SLAVE_HPP_INCLUDED
