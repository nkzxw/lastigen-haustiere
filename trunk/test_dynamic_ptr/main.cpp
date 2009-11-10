#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "slave_ptr.hpp"


struct custom_class
{
	int data_;
};

int global_int = 0;

template <typename T>
class config;

template <typename T>
class value_config_accesor
{
};


//template <typename T>
//class reference_config_accesor
//{
//public:
//	reference_config_accesor(config<T>* config)
//		: config_(config)
//	{
//		//LOCK
//		config_->mutex_.lock();
//	}
//	
//	reference_config_accesor(const reference_config_accesor& r)
//	{
//		//LOCK ????
//		// Cuidado con...
//	}
//
//	~reference_config_accesor()
//	{
//		//UNLOCK
//		config_->mutex_.unlock();
//	}
//
//private:
//	config<T>* config_;
//};

template<class T>
class ReferenceConfigAccess
{

public:
	ReferenceConfigAccess(config<T>* config) 
		: config_(config), lock_(config_->mutex_) 
	{ 
	}
	
	//T* getSettings() 
	boost::slave_ptr<T> getSettings() 
	{ 
		//return config<T>::ptr_; 
		//return config_->ptr_;
		return config_->ptr_.get();
	}

	//
	//T* operator->()  
	//{ 
	//	return getSettings(); 
	//}

private:
	//ScopedLock lock;
	boost::mutex::scoped_lock lock_;
	config<T>* config_;
};


template <typename T>
class config
{
public:

	friend class ReferenceConfigAccess<T>;

	config()
	{
		load();

		refreshThread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&config<T>::refreshMethod, this)));
	}

	void load()
	{
		ptr_.reset(new T);
		ptr_->data_ = global_int++;
	}

	void refreshMethod()
	{
		while (true)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(5000)); //TODO: especificar el tiempo por configuracion... Aunque... esta es la clase de configuracion, deberia ser por parametro en el constructor. Usando un DEFAULT-VALUE

			//TODO: ver de aplicar conditional variables
			{
	  //      boost::mutex::scoped_lock lk(mutex_);
			//std::cout << "updating ... (locked)" << std::endl;
			//boost::this_thread::sleep(boost::posix_time::milliseconds(12000));
			

			boost::master_ptr<T> newData(new T);
			newData->data_ = global_int++;
			ptr_.atomic_exchange(&newData);

			}
			//std::cout << "updated ... (unlocked)" << std::endl;
		}

	}





	//Retorna referencia haciendo Lock por bloque
	ReferenceConfigAccess<T> getAccesor()
	{
		return ReferenceConfigAccess<T>(this);
	}

	//Retorna referencia y no hace Lock por bloque
	boost::slave_ptr<T> getReference()
	{
        //boost::mutex::scoped_lock lk(mutex_);
		return ptr_.get();
	}

	T getValue()
	{
        //boost::mutex::scoped_lock lk(mutex_);
		return ptr_.get();
	}


private:
    boost::shared_ptr<boost::thread> refreshThread_;
	boost::master_ptr<T> ptr_;
	boost::mutex mutex_; //mutable 
};

//typedef config<custom_class> ch;

int main(int argc, char** argv)
{
	config<custom_class> c;
	c.load();




	//boost::slave_ptr<custom_class> ptr = c.getReference();
	
	{
	ReferenceConfigAccess<custom_class> configAccess = c.getAccesor();
	configAccess.getSettings();

	}

	//config<custom_class> c2;
	//c2.load();
	//boost::slave_ptr<custom_class> ptr2 = c2.get();
	//boost::slave_ptr<custom_class> ptr3= ptr;
	//ptr3 = ptr2;


	//while (true)
	//{
	//	//std::cout << ptr->data_ << std::endl;
	//	////std::cout << c.get()->data_ << std::endl;
	//	configAccess.getSettings();

	//	boost::this_thread::sleep(boost::posix_time::milliseconds(3000));
	//}

	

	////c.reload();
	//std::cout << ptr->data_ << std::endl;
	//std::cout << c.get()->data_ << std::endl;

	std::cin.sync();
	std::cin.get();
	return 0;

	//------------------------------------------------------------------------------------------------------------


	return 0;
}




