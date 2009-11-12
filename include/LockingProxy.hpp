#ifndef LOCKINGPROXY_HPP_INCLUDED
#define LOCKINGPROXY_HPP_INCLUDED

//#include <iostream>
//#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "slave_ptr.hpp" //TODO: tratar de incorporar este concepto a Boost


template <typename T>
class LockingProxy
{
public:

	typedef LockingProxy<T> ThisType;

	static ThisType create( boost::slave_ptr<T> ptr, boost::recursive_mutex* mutex )
	{
		return ThisType( ptr, mutex );
	}

	LockingProxy(const ThisType& other)
		: ptr_(other.ptr_), lock_(*other.mutex_), mutex_(other.mutex_)
	{ 
		//TODO: ver de usar recursive_mutex o shared_mutex para solicionar el problema de la copia...
		//TODO:
		std::cout << "WARNING: LockingProxy(const LockingProxy& other)" << std::endl;
	}
	
	~LockingProxy()
	{ 
		//TODO:
		std::cout << "WARNING: ~LockingProxy()" << std::endl;
		//mutex_.unlock();
	}
	
	//TODO: analizar cual es la razon por la cual estoy usando master_ptr y slave_ptr
	boost::slave_ptr<T> operator->()  
	{
		return ptr_; //.getSlave();
	}

private: //public: 
	LockingProxy( boost::slave_ptr<T> ptr, boost::recursive_mutex* mutex )
		: ptr_(ptr), lock_(*mutex), mutex_(mutex)
	{ 
		//mutex_.lock();
	}

	boost::recursive_mutex::scoped_lock lock_;
	boost::recursive_mutex* mutex_;
	boost::slave_ptr<T> ptr_;
};

#endif //LOCKINGPROXY_HPP_INCLUDED
