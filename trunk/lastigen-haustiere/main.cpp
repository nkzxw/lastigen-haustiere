// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

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

#include "AbstractAccessPointManager.hpp"
#include "ReflectionManager.hpp"
#include "Router.hpp"

#include "AppSettings.hpp"
#include "ConfigManager.hpp"



//bool SecurityDisabled( Router router )
//{ 
//	return (router.security_ == false);
//}


// Create a new predicate from unary_function.
template <class Arg>
struct SecurityDisabled : public std::unary_function<Arg, bool>
{
    bool operator() (const Arg& arg1) const 
	{
        //return arg1 % 2 != 0;
		return (arg1.security_ == false);
    }
};

bool SignalGreater ( Router elem1, Router elem2 )
{
   return elem1.signal_ > elem2.signal_;
}


//bool IsOdd (int i) { return ((i%2)==1); }




//#define DECLARE_SPELLABLE( a_type ) \
//template<> class spelling<a_type> { public: static const std::string
//result; }; \
//const std::string spelling<my_enum>::result = #a_type

typedef boost::scoped_ptr<AbstractAccessPointManager> APPtr;


int main(int argc, char** argv) 
{
	////TODO: pasos a seguir...
	///*
	//	1. Obtener URL del AccessPoint que contiene la lista de RoutersRemotos
	//	2. Pasarle la URL al Manager
	//	3. El manager debe obtener el HTML y parsear los datos.
	//	4. El manager debe estar en condiciones de pasar una lista limpia de RoutersRemotos
	//	5. El cliente puede solicitarle al Manager utilizar determinado RouterRemoto.
	//*/

	
	ConfigManager<AppSettings> cm(argv[0]);
	AppSettings *cfg = cm.getSettings();
	
	
	for (std::vector<AccessPointInformation>::const_iterator it = cfg->accessPoints_.begin(); it != cfg->accessPoints_.end(); ++it)
	{
		std::cout << it->name_ << std::endl;
		std::cout << it->accessPointManager_ << std::endl;

		//TODO: ver que clase puede encargarse de instanciar totalmente los APManagers...
		std::string sharedLibrary = cfg->typeMapping_[it->accessPointManager_];

		APPtr manager(ReflectionManager::CreateInstance<AbstractAccessPointManager>(sharedLibrary, it->accessPointManager_));
		//TODO: implementar un cache de DLL's ya abiertas para no reabrir la misma DLL muchas veces
	}

	APPtr manager(ReflectionManager::CreateInstance<AbstractAccessPointManager>("TpLinkManager.dll", "TpLinkManager"));


	manager->connect();
	//manager->connectTo(url);
	manager->parse("<html></html>"); // TODO: No tiene sentido, es temporal, hasta que tengamos el proceso que obtiene el html de la pagina...
	
	std::vector<Router> routers = manager->getRouterList();
	std::vector<Router> routersWithoutSecurity;

	//SecurityDisabled<Router> sd;
	//std::remove_copy_if (routers.begin(), routers.end(), std::back_inserter(routersWithoutSecurity), std::not1(SecurityDisabled<Router>()) );
	std::remove_copy_if (routers.begin(), routers.end(), std::back_inserter(routersWithoutSecurity), SecurityDisabled<Router>() );
	std::sort( routersWithoutSecurity.begin( ), routersWithoutSecurity.end( ), SignalGreater );

	//using namespace boost::lambda;
	////std::remove_copy_if(s.begin(), s.end(), std::back_inserter(t), _1 == '-');
	//std::remove_copy_if (routers.begin(), routers.end(), std::back_inserter(routersWithoutSecurity), _1 == false );

	std::cout << "-------------------------------------------------" << std::endl;
	std::copy (routersWithoutSecurity.begin(), routersWithoutSecurity.end(), std::ostream_iterator<Router>(std::cout));


	std::cin.sync();
	std::cin.get();
	return 0;
}
