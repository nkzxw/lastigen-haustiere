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


int main(int argc, char** argv) 
{
	//TODO: esto deberia ser un singleton
	//ConfigManager<AppSettings> cm(argv[0]);
	//AppSettings *cfg = cm.getSettings();

    ConfigManager<AppSettings>::lease cm;
	//cm->initialize( argv[0] );


    //ConfigManager<AppSettings>::instance->initialize( argv[0] );


	{
	//SmartNetworkManager snm(*cfg);
	SmartNetworkManager snm();
	std::cout << "--- DESTRUCTORES ---" << std::endl;
	}

	std::cin.sync();
	std::cin.get();
	return 0;
}



// --------------------------------------------------------------------------------------------------------






//// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html
//
//#include <algorithm>
//#include <functional>
//#include <iostream>
//#include <map>
//
//#include <boost/function.hpp>
////#include <boost/lambda/lambda.hpp>
//#include <boost/mpl/copy_if.hpp>
//#include <boost/scoped_ptr.hpp>
//
//#include <boost/extension/factory.hpp>
//#include <boost/extension/shared_library.hpp>
//#include <boost/extension/type_map.hpp>
//
//#include "AbstractAPController.hpp"
//#include "ReflectionManager.hpp"
//#include "Router.hpp"
//
//#include "AppSettings.hpp"
//#include "ConfigManager.hpp"
//#include "SmartNetworkManager.hpp"
//
//#include "windows.h" //TODO: eliminar
//
//
////bool SecurityDisabled( Router router )
////{ 
////	return (router.security_ == false);
////}
//
//
//// Create a new predicate from unary_function.
//template <class Arg>
//struct SecurityDisabled : public std::unary_function<Arg, bool>
//{
//    bool operator() (const Arg& arg1) const 
//	{
//        //return arg1 % 2 != 0;
//		return (arg1.security_ == false);
//    }
//};
//
//bool SignalGreater ( Router elem1, Router elem2 )
//{
//   return elem1.signal_ > elem2.signal_;
//}
//
//
////bool IsOdd (int i) { return ((i%2)==1); }
//
//
//
//
////#define DECLARE_SPELLABLE( a_type ) \
////template<> class spelling<a_type> { public: static const std::string
////result; }; \
////const std::string spelling<my_enum>::result = #a_type
//
////TODO: ver diferencias entre auto_ptr y scoped_ptr
//typedef boost::scoped_ptr<AbstractAPController> APPtr;
//
//void printRouterList(AbstractAPController *controller)
//{
//	std::vector<Router> routers = controller->getRouterList();
//	std::vector<Router> routersWithoutSecurity;
//
//	//SecurityDisabled<Router> sd;
//	//std::remove_copy_if (routers.begin(), routers.end(), std::back_inserter(routersWithoutSecurity), std::not1(SecurityDisabled<Router>()) );
//	std::remove_copy_if (routers.begin(), routers.end(), std::back_inserter(routersWithoutSecurity), SecurityDisabled<Router>() );
//	std::sort( routersWithoutSecurity.begin( ), routersWithoutSecurity.end( ), SignalGreater );
//
//	//using namespace boost::lambda;
//	////std::remove_copy_if(s.begin(), s.end(), std::back_inserter(t), _1 == '-');
//	//std::remove_copy_if (routers.begin(), routers.end(), std::back_inserter(routersWithoutSecurity), _1 == false );
//
//	std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
//	std::copy (routersWithoutSecurity.begin(), routersWithoutSecurity.end(), std::ostream_iterator<Router>(std::cout));
//}
//
//
//int main(int argc, char** argv) 
//{
//	////TODO: pasos a seguir...
//	///*
//	//	1. Obtener URL del AccessPoint que contiene la lista de RoutersRemotos
//	//	2. Pasarle la URL al Controller
//	//	3. El controller debe obtener el HTML y parsear los datos.
//	//	4. El controller debe estar en condiciones de pasar una lista limpia de RoutersRemotos
//	//	5. El cliente puede solicitarle al Controller utilizar determinado RouterRemoto.
//	//*/
//
//
//	//TODO: probar scoped_ptr y shared_ptr con puntero a memoria alocada en el Stack..
//
//
//	
//	ConfigManager<AppSettings> cm(argv[0]);
//	AppSettings *cfg = cm.getSettings();
//	
//	
//	for (std::vector<APInformation>::const_iterator it = cfg->accessPoints_.begin(); it != cfg->accessPoints_.end(); ++it)
//	{
//		std::cout << it->name_ << std::endl;
//		std::cout << it->accessPointController_ << std::endl;
//
//		//TODO: ver que clase puede encargarse de instanciar totalmente los APcontrollers...
//		//TODO: implementar un cache de DLL's ya abiertas para no reabrir la misma DLL muchas veces
//		std::string sharedLibrary = cfg->typeMapping_[it->accessPointController_];
//		APPtr controller(ReflectionManager::CreateInstance<AbstractAPController>(sharedLibrary, it->accessPointController_));
//
//		controller->initialize(*it);
//
//		//for (int i = 0; i < 100; ++i)
//		//{
//		//	printRouterList(controller.get());
//
//		//	Sleep(2000);
//		//}
//
//
//		printRouterList(controller.get());
//
//		int selection;
//
//		std::cout << "Elija el Router a utilizar: ";
//		std::cin >> selection;
//
//
//		std::cout << "El router seleccionado es: " << controller->getRouterList()[selection].bssid_ << std::endl;
//
//		controller->connectTo(manager->getRouterList()[selection]);
//
//
//	}
//
//	//APPtr controller(ReflectionManager::CreateInstance<AbstractAPController>("TpLinkController.dll", "TpLinkController"));
//
//
//
//
//	std::cin.sync();
//	std::cin.get();
//	return 0;
//}
