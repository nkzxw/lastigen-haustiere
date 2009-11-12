#ifndef CONFIGMANAGER_HPP	
#define CONFIGMANAGER_HPP

#include <fstream>

//#include <boost/any.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/utility/singleton.hpp> //torjo
//TODO: esta libreria no está soportada por boost, ver la posibilidad de implementar Singleton de Loki


//TODO: ver de usar ThreadedClass
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "ReferenceConfigAccess.hpp"

//TODO: implementar un mecanismo para refrescar la información
//TODO: evaluar Straightforward Settings de Torjo

//TODO: namespaces


//TODO: No se puede usar boost::any para poder especificar tipos dinamicos, ya que boost::any por ahora no tiene su clase serializable
typedef boost::unordered_map<std::string, std::string> KeyValueType;
//typedef boost::unordered_map<std::string, boost::any> KeyValueType;


//TODO: poner la clase en un proyecto separado, junto con ConfigurationManager.
class CommonSettings
{
public:

protected:
	friend class boost::serialization::access;

	//TODO: poner estos métodos fuera de la clase como friend... (non-intrusive)
	template < typename Archive >
	void save(Archive & ar, const unsigned int version) const //TODO: no puede ser const, porque estoy obteniendo un puntero.
	{
		//T* temp = customSettings_.get();

		//ar	& make_nvp("CommonSettings", keyValueSettings_)
		//    & make_nvp("CustomSettings", *temp ) //TODO: ver de usar operator*
		//	;

		ar	& keyValueSettings_
			;
	}

	template < typename Archive >
	void load(Archive & ar, const unsigned int version)
	{
		//T* temp = customSettings_.get();
		//ar	& make_nvp("CommonSettings", keyValueSettings_)
		//	& make_nvp("CustomSettings", *temp ) //TODO: ver de usar operator*
		//	;

		ar	& keyValueSettings_
			;

		//isDefault_ = version < 2;
	}


	BOOST_SERIALIZATION_SPLIT_MEMBER()

public: //protected:
	KeyValueType keyValueSettings_;
	//T customSettings_;
	//T* customSettings_;
	//boost::master_ptr<T> customSettings_;

};

//template <typename T>
//class CommonSettings
//{
//public:
//
//	//CommonSettings()
//	//	: customSettings_(new T)
//	//{
//	//	//customSettings_ = new T;
//	//}
//
//protected:
//	friend class boost::serialization::access;
//
//	//TODO: poner estos métodos fuera de la clase como friend... (non-intrusive)
//	template < typename Archive >
//	void save(Archive & ar, const unsigned int version) const //TODO: no puede ser const, porque estoy obteniendo un puntero.
//	{
//		//T* temp = customSettings_.get();
//
//		//ar	& make_nvp("CommonSettings", keyValueSettings_)
//		//    & make_nvp("CustomSettings", *temp ) //TODO: ver de usar operator*
//		//	;
//
//		ar	& keyValueSettings_
//			;
//	}
//
//	template < typename Archive >
//	void load(Archive & ar, const unsigned int version)
//	{
//		//T* temp = customSettings_.get();
//		//ar	& make_nvp("CommonSettings", keyValueSettings_)
//		//	& make_nvp("CustomSettings", *temp ) //TODO: ver de usar operator*
//		//	;
//
//		ar	& keyValueSettings_
//			;
//
//		//isDefault_ = version < 2;
//	}
//
//
//	BOOST_SERIALIZATION_SPLIT_MEMBER()
//
//public: //protected:
//	KeyValueType keyValueSettings_;
//	//T customSettings_;
//	//T* customSettings_;
//	//boost::master_ptr<T> customSettings_;
//
//};


// Forward declarations
#include "ConfigManager_ForwardDeclaration.hpp"


//// Locking Policies
//class NoLock;
//class Locking;
//
//// Refresing Policies
//class NoRefresh;
//class Refreshing;
//
//template <
//	typename T, 
//	typename RefreshPolicy = Refreshing
//>
//class ConfigManager;


typedef boost::mutex::scoped_lock lock;

class NoLock
{
public:
	void Policy_Method()
	{
		std::cout << "void NoLock::Policy_Method()" << std::endl;
	}

	void lock()
	{}
	void unlock()
	{}
};

class Locking
{
public:
	void Policy_Method()
	{
		std::cout << "void Locking::Policy_Method()" << std::endl;
	}
	
	void lock()
	{
		mutex_.lock();
	}
	void unlock()
	{
		mutex_.unlock();
	}

	mutable boost::mutex mutex_;

};

class NoRefresh
{
protected:
	//void refreshMethod()
	//{
	//}

	void refreshInitialize( boost::function<void()> loadFunction )
	{
	}

};



//TODO: implementar una clase en la que permitamos determinar si queremos refresco automático o no, en runtime
template <typename T>
class Refreshing
{
public:

	//TODO: poner un nombre mejor al metodo, indicando que es seguro, que se hace LOCK
	//Retorna referencia haciendo Lock por bloque
	//void getCustomSettingsLock()
	ReferenceConfigAccess<T > getCustomSettingsLock()
	{
		//TODO: para que lockear si no hay refrescoAutomatico?
		//TODO: el constructor recibe un puntero a mutex
		//return ReferenceConfigAccess<T>(this, &mutex_);
		return ReferenceConfigAccess<T>( &mutex_ );
	}


protected:

	typedef boost::function<void()> LoadFunctionType;

	void refreshInitialize( LoadFunctionType loadFunction )
	{
		loadFunction_ = loadFunction;
		refreshThread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Refreshing::refreshMethod, this)));
	}

	void refreshMethod()
	{
		while (true)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(5000)); //TODO: especificar el tiempo por configuracion... Aunque... esta es la clase de configuracion, deberia ser por parametro en el constructor. Usando un DEFAULT-VALUE
			//TODO: chequear si cambio el archivo de configuracion con algun algoritmo de hash, md5, sha.. (md5sum, shasum, etc)

			//TODO: ver de aplicar conditional variables
			{
	        lock lk(mutex_);
			//Lock();
			std::cout << "updating ... (locked)" << std::endl;					//TODO: sacar
			boost::this_thread::sleep(boost::posix_time::milliseconds(12000)); //TODO: sacar
			

			if (loadFunction_)
			{
				loadFunction_();
			}

			//load();
			//loadFunction_();
			//loadFunction();

			//Unlock();
			}
			std::cout << "updated ... (unlocked)" << std::endl;					//TODO: sacar
		}
	}


    boost::shared_ptr<boost::thread> refreshThread_;

	//TODO: mutable o no?
	mutable boost::mutex mutex_;
	//boost::mutex mutex_;

	LoadFunctionType loadFunction_;
};


//TODO: ver de cambiar por el singleton mutexed, ya que este que estamos usando no debe ser thread-safe/
//TODO: podria ponerse un callback para notificar al usuario de esta clase que la configuracion ha cambiado
//TODO: mantener una cantidad X de configuraciones previas. El X lo puede definir el usuario cuando levanta la clase (initialize)

//	typename LockingPolicy = Locking
//					 , public LockingPolicy

//template <
//	typename T, 
//	typename RefreshPolicy = Refreshing
//>


template <
	typename T, 
	template <typename> class RefreshPolicy = Refreshing
>
class ConfigManager : public boost::singleton< ConfigManager<T, RefreshPolicy> >
					 , public RefreshPolicy<T>
{
public:
	//typedef CommonSettings<T> CommonSettingsType;
	typedef ConfigManager<T, RefreshPolicy> thisType;

	//TODO: probar si es posible instanciar esta clase manualmente
	ConfigManager(boost::restricted)
		: customSettings_(new T)
	{
	}

	//TODO: hacer el "save" automaticamente en el destructor de la clase, o tambien un saver que sea automatico dentro de un thread


	void initialize( const std::string& exePath, bool loadAutomatically = true )
	{
		boost::filesystem::path path( exePath );
		configFile_ = path.replace_extension("cfg").string();

		if ( loadAutomatically )
		{
			load();
		}

		refreshInitialize( boost::bind(&thisType::load, this) );
	}



	void load()
	{
		//lock lk(mutex_); // El lock se va a hacer en la funcion que refresca, ya que load() debe ser lock-free

		std::ifstream ifs(configFile_.c_str());
		assert(ifs.good());
		boost::archive::xml_iarchive ia(ifs);

		//ia >> boost::serialization::make_nvp("Settings", settings_);
		//ia & boost::serialization::make_nvp("Settings", settings_);

		//T* temp = customSettings_.get();
		ia	>> boost::serialization::make_nvp("CommonSettings", commonSettings_)
			;

		//& boost::serialization::make_nvp("CustomSettings", *temp ) //TODO: ver de usar operator*


		//ar	& make_nvp("CommonSettings", keyValueSettings_)
		//    & make_nvp("CustomSettings", *temp ) //TODO: ver de usar operator*
		//	;
	}

	//TODO: implementar automatic save...
	void save() //const
	{
		//lock lk(mutex_);

		std::ofstream ofs(configFile_.c_str());
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);

		//oa << boost::serialization::make_nvp("Settings", settings_);

		//T* temp = customSettings_.get();
		//oa	<< boost::serialization::make_nvp("CommonSettings", commonSettings_)
		//    //& boost::serialization::make_nvp("CustomSettings", *temp ) //TODO: ver de usar operator*
		//	;
	}


	//---------------------------------------------------------------
	//TODO: ver todas las veriantes de "entrega" de datos que ConfigManager va a presentar
	// raw pointers, Locks por bloque, Locks por Get, etc...
	//---------------------------------------------------------------

	//TODO: no me gusta la forma de acceder directamente con get y set, ver de hacer un getCommonSettings, o algo parecido...
	//TODO: std::string deberia ser paramétrizable ya que podemos estar requiriendo un int, wstring, double, etc...
	//TODO: ver de retornar un puntero, referencia o bien pasar por parámetro para evitar copias innecesarias
	template <typename E>
	E get(const std::string& key) const
	{
		//TODO: para que lockear si no hay refrescoAutomatico?
		lock lk(mutex_);

		//return keyValueSettings_.at(key);
		//return boost::any_cast<E>(settings_.keyValueSettings_.at(key));
		//return boost::lexical_cast<E>(settings_.keyValueSettings_.at(key));
		return boost::lexical_cast<E>(commonSettings_.keyValueSettings_.at(key));
	}

	//void set(const std::string& key, const std::string& value)
	template <typename E>
	void set(const std::string& key, const E& value)
	{
		//TODO: para que lockear si no hay refrescoAutomatico?
		lock lk(mutex_);

		//settings_.keyValueSettings_[key] = value;
		commonSettings_.keyValueSettings_[key] = boost::lexical_cast<std::string>(value);
	}

	//TODO: slave_ptr
	T* getCustomSettings() 
	{ 
		//TODO: para que lockear si no hay refrescoAutomatico?
		//lock lk(mutex_);

		//return &settings_.customSettings_;
		//return settings_.customSettings_.get();
		return customSettings_.get();
	}

	////TODO: poner un nombre mejor al metodo, indicando que es seguro, que se hace LOCK
	////Retorna referencia haciendo Lock por bloque
	////void getCustomSettingsLock()
	//ReferenceConfigAccess<T> getCustomSettingsLock()
	//{
	//	//TODO: para que lockear si no hay refrescoAutomatico?
	//	//TODO: el constructor recibe un puntero a mutex
	//	return ReferenceConfigAccess<T>(this, &mutex_);
	//}

	//Retorna referencia y no hace Lock por bloque
	boost::slave_ptr<T> getCustomSettingsReference()
	{
        //boost::mutex::scoped_lock lk(mutex_);
		//return settings_.customSettings_.getSlave();
		return customSettings_.getSlave();
	}

protected:
	std::string configFile_;

	//TODO: los customSettings están dentro de la clase CommonSettings solo porque necesito serializarla. Deberían estar fuera
	//CommonSettings<T> settings_;
	CommonSettings commonSettings_; //TODO: implementar puntero...
	boost::master_ptr<T> customSettings_;
};





#endif // CONFIGMANAGER_HPP
