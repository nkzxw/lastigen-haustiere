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

//#include <boost/utility/singleton.hpp> //tobias class (torjo)
//#include <boost/utility/mutexed_singleton.hpp> //tobias class (torjo)


//TODO: ver si agrego una clase Application, que se encargue del ConfigManager y el Logging. Algo comun en casi todo tipo de aplicaciones. Ver de usar boost::program_options
int main(int argc, char** argv) 
{
	ConfigManager<AppSettings>::instance->initialize( argv[0] );

	{
		SmartNetworkManager snm; //una vez que esta clase sale de scope, se destruyen automáticamente todos los threads creados por ella.

		std::cout << "press Ctrl-c to exit..." << std::endl;

		while (true) // check the internal state of the connection to make sure it's still running
		{
			//boost::this_thread::sleep(boost::posix_time::milliseconds(300));

			char ch;
			cin.get(ch); // blocking wait for standard input
			//if (ch == 3) // ctrl-C to end program
			if (ch == 'z') // ctrl-C to end program
			{
				break;
			}
		}

		std::cout << "--- DESTRUCTORES ---" << std::endl;
	}

	std::cin.sync();
	std::cin.get();
	return 0;
}
