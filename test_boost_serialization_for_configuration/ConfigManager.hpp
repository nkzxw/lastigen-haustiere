#ifndef CONFIGMANAGER_HPP	
#define CONFIGMANAGER_HPP

#include <fstream>
#include <boost/filesystem/path.hpp>

template <typename T>
class ConfigManager
{
public:
	ConfigManager(const std::string& exePath, bool loadAutomatically = false )
	{
		
		boost::filesystem::path my_path( exePath );
		configFile_ = my_path.replace_extension("cfg").string();

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

		ia >> BOOST_SERIALIZATION_NVP(configurationClass_);
	}

	void save()
	{
		std::ofstream ofs(configFile_.c_str());
		assert(ofs.good());
		boost::archive::xml_oarchive oa(ofs);

		oa << BOOST_SERIALIZATION_NVP(configurationClass_);
	}

	//T& getConfigClass()
	//{
	//	return configurationClass_;
	//}

	T* operator->() 
	{ 
		return &configurationClass_;
		//return temp;
		//return *p; 
		//return configurationClass_; 
	}


protected:
	std::string configFile_;
	T configurationClass_;
};

#endif // CONFIGMANAGER_HPP
