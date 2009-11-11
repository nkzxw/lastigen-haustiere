#ifndef REFERENCECONFIGACCESS
#define REFERENCECONFIGACCESS

//#include <iostream>
//#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "slave_ptr.hpp"


//template <typename T>
//class ConfigManager;

class NoLock;

template <typename T, typename LockingPolicy = NoLock>
class ConfigManager;


//LockProxy
//TODO: el nombre tendria que aclarar que se va a Lockear
template<class T>
class ReferenceConfigAccess
{

public:

	friend class ConfigManager<T>;

	ReferenceConfigAccess(const ReferenceConfigAccess& other) 
	{ 
		//TODO:
		std::cout << "WARNING: ReferenceConfigAccess(const ReferenceConfigAccess& other)" << std::endl;
	}
	
	~ReferenceConfigAccess() 
	{ 
		//TODO:
		std::cout << "WARNING: ~ReferenceConfigAccess()" << std::endl;
		//mutex_.unlock();
	}
	
	boost::slave_ptr<T> operator->()  
	{ 
		//return config_->ptr_.get();
		return config_->getCustomSettingsReference();
		
	}

private:
	ReferenceConfigAccess(ConfigManager<T>* config, boost::mutex* mutex) 
		: mutex_(mutex), config_(config), lock_(*mutex) 
	{ 
		//mutex_.lock();
	}

	boost::mutex::scoped_lock lock_;
	boost::mutex* mutex_;
	
	//TODO: ver si la clase ConfigManager podría ser genérica, ya que estoy atado a ella.
	ConfigManager<T>* config_;
};

#endif //REFERENCECONFIGACCESS
