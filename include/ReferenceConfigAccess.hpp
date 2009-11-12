#ifndef REFERENCECONFIGACCESS
#define REFERENCECONFIGACCESS

//#include <iostream>
//#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "slave_ptr.hpp"
#include "ConfigManager_ForwardDeclaration.hpp"


//TODO: hacer una clase Factory para crear estos objetos. Esta clase Factory tiene que ser friend de ReferenceConfigAccess, asi puedo poner el constructor de ReferenceConfigAccess privado. Ver si se justifica hacer esto.

//LockProxy
//TODO: el nombre tendria que aclarar que se va a Lockear
template <typename T>
class ReferenceConfigAccess
{
public:
	////TODO:
	//friend class ConfigManager<T>;
	////friend class ConfigManager<T, RefreshPolicy>;


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
	
	//TODO: analizar cual es la razon por la cual estoy usando master_ptr y slave_ptr
	boost::slave_ptr<T> operator->()  
	{
		//return config_->getCustomSettingsReference();
		return ptr_.getSlave();
	}

public: //private:
	//ReferenceConfigAccess(ConfigManager<T>* config, boost::mutex* mutex) 
	//	: mutex_(mutex), config_(config), lock_(*mutex) 
	//{ 
	//	//mutex_.lock();
	//}

	ReferenceConfigAccess( boost::master_ptr<T> ptr, boost::mutex* mutex )
		: ptr_(ptr), mutex_(mutex), lock_(*mutex) 
	{ 
		//mutex_.lock();
	}


private:
	boost::mutex::scoped_lock lock_;
	boost::mutex* mutex_;
	boost::master_ptr<T> ptr_;
	
	//TODO: ver si la clase ConfigManager podría ser genérica, ya que estoy atado a ella.
	//ConfigManager<T>* config_;
};

#endif //REFERENCECONFIGACCESS
