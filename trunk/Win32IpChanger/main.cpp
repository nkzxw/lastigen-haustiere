
#include <iostream>	//TODO: quitar

#include <boost/extension/extension.hpp>
#include <boost/extension/factory.hpp>
#include <boost/extension/type_map.hpp>

#include "AbstractIpChanger.hpp"

#ifdef _WIN32
#include "Win32IpChanger.hpp"
typedef Win32IpChanger IpChanger;
#else
#endif


using namespace boost::extensions;

////typedef factory<AbstractIpChanger, int> FactoryType;
//typedef factory<AbstractIpChanger, void> FactoryType;
typedef factory<AbstractIpChanger> FactoryType;
typedef std::map<std::string, FactoryType> FactoryMap;

//BOOST_EXTENSION_TYPE_MAP_FUNCTION 
//{
//	FactoryMap& factories(types.get());
//	factories["Win32IpChanger_factory"].set<Win32IpChanger>();
//}

int main()
{
	IpChanger changer;

	std::cout << changer.setStatic("EstoTieneQueDarError", "192.168.111.111", "255.255.255.0", "192.168.111.1");
	std::cout << changer.getMessage();

	std::cout << changer.setStatic("Local Area Connection", "192.168.111.111", "255.255.255.0", "192.168.111.1");
	std::cout << changer.setDynamic("Local Area Connection");

}