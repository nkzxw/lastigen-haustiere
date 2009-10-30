#ifndef CONFIGMANAGER_HPP	
#define CONFIGMANAGER_HPP

#include <fstream>

//#include <boost/any.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/utility/singleton.hpp> //torjo
//TODO: esta libreria no está soportada por boost, ver la posibilidad de implementar Singleton de Loki


//TODO: ver de usar ThreadedClass
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>


//TODO: implementar un mecanismo para refrescar la información
//TODO: evaluar Straightforward Settings de Torjo

//TODO: namespaces


//TODO: No se puede usar boost::any para poder especificar tipos dinamicos, ya que boost::any por ahora no tiene su clase serializable
typedef boost::unordered_map<std::string, std::string> KeyValueType;
//typedef boost::unordered_map<std::string, boost::any> KeyValueType;


//TODO: poner la clase en un proyecto separado, junto con ConfigurationManager.
template <typename T>
class CommonSettings
{
public:


protected:

	friend class boost::serialization::access;

	//TODO: poner estos métodos fuera de la clase como friend...
	template < typename Archive >
	void save(Archive & ar, const unsigned int version) const
	{
		ar	& make_nvp("CommonSettings", keyValueSettings_)
		    & make_nvp("CustomSettings", customSettings_)
			;
	}

	template < typename Archive >
	void load(Archive & ar, const unsigned int version)
	{
		
		ar	& make_nvp("CommonSettings", keyValueSettings_)
			& make_nvp("CustomSettings", customSettings_)
			;

		//isDefault_ = version < 2;
	}


	BOOST_SERIALIZATION_SPLIT_MEMBER()

public: //protected:
	KeyValueType keyValueSettings_;
	T customSettings_;
};




typedef boost::mutex::scoped_lock lock;

//TODO: ver de cambiar por el singleton mutexed, ya que este que estamos usando no debe ser thread-safe/
//TODO: poner un atributo "autoRefresh". En caso de ser TRUE, levantar un hilo que se encargue de refrescar la configuración.
//TODO: podria ponerse un callback para notificar al usuario de esta clase que la configuracion ha cambiado
//TODO: mantener una cantidad X de configuraciones previas. El X lo puede definir el usuario cuando levanta la clase (initialize)
template <typename T>
class ConfigManager : public boost::singleton< ConfigManager<T> >
{
public:
	//typedef CommonSettings<T> CommonSettingsType;

	//TODO: probar si es posible instanciar esta clase manualmente
	ConfigManager(boost::restricted)
	{}

	//TODO: hacer el "save" automaticamente en el destructor de la clase, o tambien un saver que sea automatico dentro de un thread

	void initialize( const std::string& exePath, bool loadAutomatically = true, bool automaticRefresh = true )
	{
		boost::filesystem::path path( exePath );
		configFile_ = path.replace_extension("cfg").string();

		if ( loadAutomatically )
		{
			load();
		}

		if (automaticRefresh)
		{
			//refreshThread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&ThreadedClass::doWork, this)));
			refreshThread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&ConfigManager<T>::refreshMethod, this)));
		}

	}

	void refreshMethod()
	{
		while (true)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(10000));
			//TODO: chequear si cambio el archivo de configuracion con algun algoritmo de hash, md5, sha.. (md5sum, shasum, etc)

			//TODO: ver de aplicar conditional variables
			{
	        lock lk(mutex_);
			load();
			}
		}
	}

	void load()
	{
		//lock lk(mutex_);

		std::ifstream ifs(configFile_.c_str());
		assert(ifs.good());
		boost::archive::xml_iarchive ia(ifs);

		ia >> boost::serialization::make_nvp("Settings", settings_);
	}

	void save()
	{
		//lock lk(mutex_);

		std::ofstream ofs(configFile_.c_str());
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);

		oa << boost::serialization::make_nvp("Settings", settings_);
	}

	//T& getConfigClass()
	//{
	//	return configurationClass_; //TODO: ?????????????????????????
	//}

	//TODO: ver de retornar una referencia en vez de un puntero.
	T* getSettings() 
	{ 
		lock lk(mutex_);

		//return &settings_;
		return &settings_.customSettings_;
	}

	////TODO: directamente podria llamarse get y set... que estos apunten directamente a CommonSettings
	//CommonSettings<T>* getCommonSettings() 
	//{ 
	//	return &settings_;
	//}


	//TODO: std::string deberia ser paramétrizable ya que podemos estar requiriendo un int, wstring, double, etc...
	//TODO: ver de retornar un puntero, referencia o bien pasar por parámetro para evitar copias innecesarias
	template <typename E>
	E get(const std::string& key) const
	{
		lock lk(mutex_);

		//return keyValueSettings_.at(key);
		//return boost::any_cast<E>(settings_.keyValueSettings_.at(key));
		return boost::lexical_cast<E>(settings_.keyValueSettings_.at(key));
	}

	//void set(const std::string& key, const std::string& value)
	template <typename E>
	void set(const std::string& key, const E& value)
	{
		lock lk(mutex_);

		//settings_.keyValueSettings_[key] = value;
		settings_.keyValueSettings_[key] = boost::lexical_cast<std::string>(value);
	}

	T* getCustomSettings() 
	{ 
		lock lk(mutex_);

		return &settings_.customSettings_;
	}

protected:
	std::string configFile_;
	//T settings_;
	CommonSettings<T> settings_;

    boost::shared_ptr<boost::thread> refreshThread_;
    mutable boost::mutex mutex_;

};





#endif // CONFIGMANAGER_HPP
