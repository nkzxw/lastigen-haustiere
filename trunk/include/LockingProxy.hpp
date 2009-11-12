#ifndef LOCKINGPROXY_HPP_INCLUDED
#define LOCKINGPROXY_HPP_INCLUDED

//#include <iostream>
//#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "slave_ptr.hpp"
#include "ConfigManager_ForwardDeclaration.hpp"


//LockProxy
//TODO: el nombre tendria que aclarar que se va a Lockear
template <typename T>
class LockingProxy
{
public:

	static LockingProxy<T> create( boost::master_ptr<T> ptr, boost::recursive_mutex* mutex )
	{
		return LockingProxy<T>( ptr, mutex );
	}

	LockingProxy(const LockingProxy& other)
		: ptr_(other.ptr_), mutex_(other.mutex_), lock_(*other.mutex_) 
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
		return ptr_.getSlave();
	}

private:
	LockingProxy( boost::master_ptr<T> ptr, boost::recursive_mutex* mutex )
		: ptr_(ptr), mutex_(mutex), lock_(*mutex) 
	{ 
		//mutex_.lock();
	}


private:
	boost::recursive_mutex::scoped_lock lock_;
    
	boost::recursive_mutex* mutex_;
	boost::master_ptr<T> ptr_;
	
};

#endif //LOCKINGPROXY_HPP_INCLUDED
