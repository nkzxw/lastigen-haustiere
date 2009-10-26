#ifndef CONFIGMANAGER_HPP	
#define CONFIGMANAGER_HPP

#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/utility/singleton.hpp> //torjo
//TODO: esta libreria no está soportada por boost, ver la posibilidad de implementar Singleton de Loki


//TODO: implementar un mecanismo para refrescar la información
//TODO: evaluar Straightforward Settings de Torjo


//TODO: ver de cambiar por el singleton mutexed, ya que este que estamos usando no debe ser thread-safe
template <typename T>
class ConfigManager : public boost::singleton< ConfigManager<T> >
{
public:
	ConfigManager(boost::restricted)
	{
	}

	void initialize( const std::string& exePath, bool loadAutomatically = true )
	{
		boost::filesystem::path path( exePath );
		configFile_ = path.replace_extension("cfg").string();

		if ( loadAutomatically )
		{
			load();
		}
	}

	void load()
	{
		std::ifstream ifs(configFile_.c_str());
		assert(ifs.good());
		boost::archive::xml_iarchive ia(ifs);

		ia >> boost::serialization::make_nvp("Settings", settings_);
		
	}

	void save()
	{
		std::ofstream ofs(configFile_.c_str());
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);

		oa << boost::serialization::make_nvp("Settings", settings_);
	}

	T& getConfigClass()
	{
		return configurationClass_;
	}

	//TODO: ver de retornar una referencia en vez de un puntero.
	T* getSettings() 
	{ 
		return &settings_;
		//return temp;
		//return *p; 
		//return configurationClass_; 
	}


protected:
	std::string configFile_;
	T settings_;
};





#endif // CONFIGMANAGER_HPP
