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

#include "AbstractRouterManager.hpp"
#include "ReflectionManager.hpp"
#include "Router.hpp"



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




int main() 
{
	//using namespace boost::extensions;

	//typedef factory<AbstractRouterManager, void> FactoryType;
	//typedef std::map<std::string, FactoryType> FactoryMap;

	//std::string library_path = "TPLinkManager.dll";

	//// Create shared_library object with the relative or absolute
	//// path to the shared library.
	//shared_library lib(library_path);

	//// Attempt to open the shared library.
	//if (!lib.open()) 
	//{
	//	std::cerr << "Library failed to open: " << library_path << std::endl;
	//	return 1;
	//}

	//// Use the shared_library::call to automatically call an Extension-specific function in the shared library,
	//// which takes a type_map.
	//type_map types;
	//if (!lib.call(types)) 
	//{
	//	std::cerr << "Function not found!" << std::endl;
	//	return 1;
	//}

	//// Retrieve a map of all animal factories taking an int and indexed by a string from the type_map.
	//FactoryMap& factories(types.get());
	//if (factories.empty()) 
	//{
	//	std::cerr << "Animals not found!" << std::endl;
	//	return 1;
	//}

	//if ( factories.find("factory") == factories.end() )								//TODO: ver si es la forma correcta de chequear que existe el key
	//{
	//	std::cerr << "Error in Shared Library. No Factory founded." << std::endl;
	//	return 1;
	//}

	////TODO: pasos a seguir...
	///*
	//	1. Obtener URL del AccessPoint que contiene la lista de RoutersRemotos
	//	2. Pasarle la URL al Manager
	//	3. El manager debe obtener el HTML y parsear los datos.
	//	4. El manager debe estar en condiciones de pasar una lista limpia de RoutersRemotos
	//	5. El cliente puede solicitarle al Manager utilizar determinado RouterRemoto.
	//*/

	//boost::scoped_ptr<AbstractRouterManager> manager(factories["factory"].create());		//TODO: investigar scoped_ptr


	ReflectionManager rm;
	//boost::scoped_ptr<AbstractRouterManager> manager = rm.get<AbstractRouterManager>();
	boost::scoped_ptr<AbstractRouterManager> manager(rm.get<AbstractRouterManager>());



	//manager->connectTo(url);
	manager->parse("<html></html>"); // No tiene sentido, es temporal, hasta que tengamos el proceso que obtiene el html de la pagina...
	
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
