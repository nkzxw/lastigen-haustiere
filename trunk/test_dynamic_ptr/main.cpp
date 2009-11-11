#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "slave_ptr.hpp"
#include "ReferenceConfigAccess.hpp"

struct custom_class
{
	int data_;
};

int global_int = 0;

template <typename T>
class config;

//TODO: lo implemento?
//template <typename T>
//class ValueConfigAccess
//{
//};


template <typename T>
class config
{
public:

	//friend class ReferenceConfigAccess<T>;

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
			std::cout << "waiting to lock ... " << std::endl;
	        boost::mutex::scoped_lock lk(mutex_);
			std::cout << "updating ... (locked)" << std::endl;
			//boost::this_thread::sleep(boost::posix_time::milliseconds(12000));

			boost::master_ptr<T> newData(new T);
			newData->data_ = global_int++;
			ptr_.atomic_exchange(&newData);

			}
			std::cout << "updated ... (unlocked)" << std::endl;
		}
	}

	//Retorna referencia haciendo Lock por bloque
	ReferenceConfigAccess<T> getAccesor()
	{
		return ReferenceConfigAccess<T>(this, mutex_);
	}

	//Retorna referencia y no hace Lock por bloque
	boost::slave_ptr<T> getReference()
	{
        //boost::mutex::scoped_lock lk(mutex_);
		return ptr_.get();
	}

	T getValue()
	{
		boost::mutex::scoped_lock lk(mutex_);
		return *ptr_.get();
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
	
	boost::slave_ptr<custom_class> outScopePtr;

	while (true)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
		{
		std::cout << "trying to access to data..." << std::endl;
		ReferenceConfigAccess<custom_class> configAccess = c.getAccesor();

		std::cout << "begin data accessing ... (locked)" << std::endl;
		outScopePtr = configAccess.operator ->();

		//boost::this_thread::sleep(boost::posix_time::milliseconds(2000));

		std::cout << "Refresh: " << configAccess->data_ << std::endl;
		}
		std::cout << "end data accessing ... (unlocked)" << std::endl;


		std::cout << "********* WARNING UNSECURED DATA (no-lock): "  << outScopePtr->data_ << std::endl;


		boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
		{
		std::cout << "trying to access to data..." << std::endl;
		std::cout << "begin data accessing ... (locked)" << std::endl;
		custom_class tempValue = c.getValue();
		std::cout << "end data accessing ... (unlocked)" << std::endl;

		boost::this_thread::sleep(boost::posix_time::milliseconds(2000));

		std::cout << "Value: " << tempValue.data_ << std::endl;
		}



	}


	std::cin.sync();
	std::cin.get();
	return 0;

	//------------------------------------------------------------------------------------------------------------


	return 0;
}




