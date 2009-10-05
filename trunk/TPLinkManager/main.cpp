// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

//TODO: ver
#include <iostream>
#include <map>

#include <boost/extension/extension.hpp>
#include <boost/extension/factory.hpp>
#include <boost/extension/type_map.hpp>

#include "TpLinkManager.hpp"
#include "appsettings.hpp"

using namespace boost::extensions;



////typedef factory<AbstractAPManager, int> FactoryType;
//typedef factory<AbstractAPManager, void> FactoryType;
//typedef factory<AbstractAPManager> FactoryType;
typedef boost::extensions::factory<AbstractAPManager, AccessPointInformation> FactoryType;
typedef std::map<std::string, FactoryType> FactoryMap;


BOOST_EXTENSION_TYPE_MAP_FUNCTION 
{
	FactoryMap& factories(types.get());
	factories["TpLinkManager_factory"].set<TpLinkManager>();
}
