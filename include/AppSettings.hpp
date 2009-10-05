// http://landheer-cieslak.com/wordpress/?p=130
// Se generó archivo <boost/serialization/unordered_map.hpp>

#ifndef APPSETTINGS_HPP
#define APPSETTINGS_HPP

//#include <fstream>
//#include <iostream>

#include <vector>
#include <boost/unordered_map.hpp> 

#include <boost/filesystem/path.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>						//BOOST_SERIALIZATION_SPLIT_MEMBER()

#include <boost/serialization/vector.hpp>
//#include <boost/serialization/map.hpp>
#include <boost/serialization/unordered_map.hpp>

#include <boost/serialization/version.hpp>							//BOOST_CLASS_VERSION

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>

using namespace boost::serialization;

typedef boost::unordered_map<std::string, std::string> MapType;


//TODO: ver de hacerla heredar de alguna clase comun... 
//TODO: ... o algo asi como dividirla en Sections o similar... (generica)
class AccessPointInformation
{
public:
	AccessPointInformation()
		: name_(""), accessPointManager_(""), routerListUri_(""), useRouterUri_(""), httpBasicCredentials_("")
	{}

	AccessPointInformation(const std::string& name, const std::string& accessPointManager, const std::string& routerListUri, const std::string& useRouterUri, const std::string& httpBasicCredentials)
		: name_(name), accessPointManager_(accessPointManager), routerListUri_(routerListUri), useRouterUri_(useRouterUri), httpBasicCredentials_(httpBasicCredentials)
	{}


protected:
	friend class boost::serialization::access;

	//TODO: poner estos métodos fuera de la clase como friend...
	template < typename Archive >
	void save(Archive & ar, const unsigned int version) const
	{
		//TODO: cambiar los nombres en el xml
		ar	& BOOST_SERIALIZATION_NVP(name_)
			& BOOST_SERIALIZATION_NVP(accessPointManager_)
			& BOOST_SERIALIZATION_NVP(routerListUri_)
			& BOOST_SERIALIZATION_NVP(useRouterUri_)
			& BOOST_SERIALIZATION_NVP(httpBasicCredentials_)
			;
		//ar & make_nvp("id", id_);
	}

	template < typename Archive >
	void load(Archive & ar, const unsigned int version)
	{
		ar	& BOOST_SERIALIZATION_NVP(name_)
			& BOOST_SERIALIZATION_NVP(accessPointManager_)
			& BOOST_SERIALIZATION_NVP(routerListUri_)
			& BOOST_SERIALIZATION_NVP(useRouterUri_)
			& BOOST_SERIALIZATION_NVP(httpBasicCredentials_)
			;

		//isDefault_ = version < 2;
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

public: //TODO: ver de hacer metodos para acceder a los datos
	std::string name_;
	std::string accessPointManager_;
	std::string routerListUri_;
	std::string useRouterUri_;
	std::string httpBasicCredentials_;
	//TODO: otros datos
};


class AppSettings //: public MappingConfiguration
{
public:

	AppSettings()
		: isDefault_(true)
		//, dllPath_("")
	{ 
		/* no-op */ 
	}


protected:

	friend class boost::serialization::access;

	//TODO: poner estos métodos fuera de la clase como friend...
	template < typename Archive >
	void save(Archive & ar, const unsigned int version) const
	{
		ar	& make_nvp("AccessPoints", accessPoints_)
			& make_nvp("TypeMapping", typeMapping_)
			;
	}

	template < typename Archive >
	void load(Archive & ar, const unsigned int version)
	{
		
		ar	& make_nvp("AccessPoints", accessPoints_)
			& make_nvp("TypeMapping", typeMapping_)
			;

		isDefault_ = version < 2;
	}


	BOOST_SERIALIZATION_SPLIT_MEMBER()

protected:


public: //protected:

	MapType typeMapping_;
	std::vector<AccessPointInformation> accessPoints_;

	bool isDefault_;
};


BOOST_CLASS_VERSION(AppSettings, 0);
BOOST_CLASS_VERSION(AccessPointInformation, 0);

#endif // APPSETTINGS_HPP
