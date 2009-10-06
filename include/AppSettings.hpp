// http://landheer-cieslak.com/wordpress/?p=130
// Se generó archivo <boost/serialization/unordered_map.hpp>

//TODO: cambiar nombre de archivo

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

#include "Router.hpp"

using namespace boost::serialization;

typedef boost::unordered_map<std::string, std::string> MapType;


//TODO: ver de hacerla heredar de alguna clase comun... 
//TODO: ... o algo asi como dividirla en Sections o similar... (generica)
//TODO: poner esta clase en otro archivo, crear un proyecto aparte con para la configuracion
class APInformation
{
public:
	APInformation()
		: name_(""), accessPointManager_(""), protocol_(""), host_(""), httpBasicCredentials_("")
	{}

	APInformation(const std::string& name, const std::string& accessPointManager, const std::string& protocol, const std::string& host, const std::string& httpBasicCredentials)
		: name_(name), accessPointManager_(accessPointManager), protocol_(protocol), host_(host), httpBasicCredentials_(httpBasicCredentials)
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
			& BOOST_SERIALIZATION_NVP(protocol_)
			& BOOST_SERIALIZATION_NVP(host_)
			& BOOST_SERIALIZATION_NVP(httpBasicCredentials_)
			;
		//ar & make_nvp("id", id_);
	}

	template < typename Archive >
	void load(Archive & ar, const unsigned int version)
	{
		ar	& BOOST_SERIALIZATION_NVP(name_)
			& BOOST_SERIALIZATION_NVP(accessPointManager_)
			& BOOST_SERIALIZATION_NVP(protocol_)
			& BOOST_SERIALIZATION_NVP(host_)
			& BOOST_SERIALIZATION_NVP(httpBasicCredentials_)
			;

		//isDefault_ = version < 2;
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

public: //TODO: ver de hacer metodos para acceder a los datos
	std::string name_;
	std::string accessPointManager_;

	////TODO: separar lo que es protocolo y direccion IP de las rutas para acceder a cada funcionalidad.
	////TODO: las rutas para cada funcionalidad son rutas internas de cada AP y no deberían ser configurables por el usuario... es configuracion, pero a otro nivel.
	//std::string routerListUri_;
	//std::string useRouterUri_;


	//TODO: separar lo que es protocolo y direccion IP de las rutas para acceder a cada funcionalidad.
	//TODO: las rutas para cada funcionalidad son rutas internas de cada AP y no deberían ser configurables por el usuario... es configuracion, pero a otro nivel.
	std::string protocol_;
	std::string host_;
	//TODO: port

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
			& make_nvp("KnownRouters", knownRouters_)
			;
	}

	template < typename Archive >
	void load(Archive & ar, const unsigned int version)
	{
		
		ar	& make_nvp("AccessPoints", accessPoints_)
			& make_nvp("TypeMapping", typeMapping_)
			& make_nvp("KnownRouters", knownRouters_)
			;

		isDefault_ = version < 2;
	}


	BOOST_SERIALIZATION_SPLIT_MEMBER()

protected:


public: //protected:

	MapType typeMapping_;
	std::vector<APInformation> accessPoints_;

	//TODO: cambiar vector por map
	std::vector<Router> knownRouters_;	//TODO: poner una prioridad o un nivel de preferencia
	bool isDefault_;
};


BOOST_CLASS_VERSION(AppSettings, 0);
BOOST_CLASS_VERSION(APInformation, 0);

#endif // APPSETTINGS_HPP
