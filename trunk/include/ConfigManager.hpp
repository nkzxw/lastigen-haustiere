#ifndef CONFIGMANAGER_HPP	
#define CONFIGMANAGER_HPP

#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/utility/singleton.hpp> //torjo


template <typename T>
class ConfigManager : public boost::singleton< ConfigManager<T> >
{
public:
	ConfigManager(boost::restricted);

	//ConfigManager(const std::string& exePath, bool loadAutomatically = true )
	//{
	//	boost::filesystem::path path( exePath );
	//	configFile_ = path.replace_extension("cfg").string();

	//	if ( loadAutomatically )
	//	{
	//		load();
	//	}
	//}

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

	//T& getConfigClass()
	//{
	//	return configurationClass_;
	//}

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


//template <typename T>
//class ConfigManager
//{
//public:
//	ConfigManager(const std::string& exePath, bool loadAutomatically = true )
//	{
//		
//		boost::filesystem::path my_path( exePath );
//		configFile_ = my_path.replace_extension("cfg").string();
//
//		if ( loadAutomatically )
//		{
//			load();
//		}
//	}
//
//	void load()
//	{
//		std::ifstream ifs(configFile_.c_str());
//		assert(ifs.good());
//		boost::archive::xml_iarchive ia(ifs);
//
//		ia >> BOOST_SERIALIZATION_NVP(configurationClass_);
//	}
//
//	void save()
//	{
//		std::ofstream ofs(configFile_.c_str());
//		assert(ofs.good());
//		boost::archive::xml_oarchive oa(ofs);
//
//		oa << BOOST_SERIALIZATION_NVP(configurationClass_);
//	}
//
//	//T& getConfigClass()
//	//{
//	//	return configurationClass_;
//	//}
//
//	T* operator->() 
//	{ 
//		return &configurationClass_;
//		//return temp;
//		//return *p; 
//		//return configurationClass_; 
//	}
//
//
//protected:
//	std::string configFile_;
//	T configurationClass_;
//};
//





#endif // CONFIGMANAGER_HPP
