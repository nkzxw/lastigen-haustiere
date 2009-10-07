// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

//TODO: ver
#include <iostream>
#include <map>

#include <boost/extension/extension.hpp>
#include <boost/extension/factory.hpp>
#include <boost/extension/type_map.hpp>

#include "TpLinkController.hpp"
#include "appsettings.hpp"

using namespace boost::extensions;



////typedef factory<AbstractAPController, int> FactoryType;
//typedef factory<AbstractAPController, void> FactoryType;
//typedef boost::extensions::factory<AbstractAPController, APInformation> FactoryType;
typedef boost::extensions::factory<AbstractAPController> FactoryType;
typedef std::map<std::string, FactoryType> FactoryMap;


BOOST_EXTENSION_TYPE_MAP_FUNCTION 
{
	FactoryMap& factories(types.get());
	factories["TpLinkController_factory"].set<TpLinkController>();
}
