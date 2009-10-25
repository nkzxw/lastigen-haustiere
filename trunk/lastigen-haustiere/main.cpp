// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html
// SINGLETON: http://torjo.com/tobias/


#include <algorithm>
#include <functional>
#include <iostream>
#include <map>

#include <boost/function.hpp>
//#include <boost/lambda/lambda.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/extension/factory.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/extension/type_map.hpp>

#include "AbstractAPController.hpp"
#include "ReflectionManager.hpp"
#include "Router.hpp"

#include "AppSettings.hpp"
#include "ConfigManager.hpp"
#include "SmartNetworkManager.hpp"

#include "windows.h" //TODO: eliminar


//#include <boost/utility/singleton.hpp> //torjo
#include <boost/utility/mutexed_singleton.hpp> //torjo



//TODO: ver si agrego una clase Application, que se encargue del ConfigManager y el Logging. Algo comun en casi todo tipo de aplicaciones. Ver de usar boost::program_options
int main(int argc, char** argv) 
{
 //   ConfigManager<AppSettings>::lease cm;
	//cm->initialize( argv[0] );
	ConfigManager<AppSettings>::instance->initialize( argv[0] );
	//AppSettings *cfg = cm.getSettings();

	{
	SmartNetworkManager snm;
	std::cout << "--- DESTRUCTORES ---" << std::endl;
	}

	std::cin.sync();
	std::cin.get();
	return 0;
}
