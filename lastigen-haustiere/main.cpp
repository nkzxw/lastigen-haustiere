// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

#include <iostream>
#include <map>

#include <boost/extension/factory.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/extension/type_map.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include "AbstractRouterManager.hpp"



int main() 
{
	using namespace boost::extensions;

	typedef factory<AbstractRouterManager, void> FactoryType;
	typedef std::map<std::string, FactoryType> FactoryMap;

	std::string library_path = "TPLinkManager.dll";

	// Create shared_library object with the relative or absolute
	// path to the shared library.
	shared_library lib(library_path);

	// Attempt to open the shared library.
	if (!lib.open()) 
	{
		std::cerr << "Library failed to open: " << library_path << std::endl;
		return 1;
	}

	// Use the shared_library::call to automatically call an Extension-specific function in the shared library,
	// which takes a type_map.
	type_map types;
	if (!lib.call(types)) 
	{
		std::cerr << "Function not found!" << std::endl;
		return 1;
	}


	// Retrieve a map of all animal factories taking an int and indexed by a string from the type_map.
	FactoryMap& factories(types.get());
	if (factories.empty()) 
	{
		std::cerr << "Animals not found!" << std::endl;
		return 1;
	}



	if ( factories.find("factory") == factories.end() )								//TODO: ver si es la forma correcta de chequear que existe el key
	{
		std::cerr << "Error in Shared Library. No Factory founded." << std::endl;
		return 1;
	}



	//TODO: pasos a seguir...
	/*
		1. Obtener URL del AccessPoint que contiene la lista de RoutersRemotos
		2. Pasarle la URL al Manager
		3. El manager debe obtener el HTML y parsear los datos.
		4. El manager debe estar en condiciones de pasar una lista limpia de RoutersRemotos
		5. El cliente puede solicitarle al Manager utilizar determinado RouterRemoto.
	*/

	boost::scoped_ptr<AbstractRouterManager> manager(factories["factory"].create());		//TODO: investigar scoped_ptr
	//manager->connectTo(url);
	manager->parse("<html></html>"); // No tiene sentido, es temporal, hasta que tengamos el proceso que obtiene el html de la pagina...
	manager->getList();

	std::cin.sync();
	std::cin.get();
	return 0;
}




//#include <iostream>
//#include <string>
//


//#include <boost/regex.hpp>
//
//// libboost_regex-vc90-mt-gd-1_40.lib
//// libboost_regex-vc90-mt-gd-1_39.lib
//
//// http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid
//
//
//bool validate_card_format(const std::string& s)
//{
//   static const boost::regex e("(\\d{4}[- ]){3}\\d{4}");
//   return regex_match(s, e);
//}
//
//
//int main(int argc, char **argv)
//{
//	char tempChar[20];
//
//	std::cin.getline(tempChar, 20);
//	std::string creditCardNumber(tempChar);
//	bool isValid = validate_card_format(creditCardNumber);
//
//	std::cout << creditCardNumber << " ... is valid: " << isValid << std::endl;
//
//	std::cin.sync();
//	std::cin.get();
//	return 0;
//}





//#include <iostream>
//#include <boost/regex.hpp>
//
//using namespace std;
//
//int main(int argc)
//{
//   string s;
//   do{
//      if(argc == 1)
//      {
//         cout << "Enter text to split (or \"quit\" to exit): ";
//         getline(cin, s);
//         if(s == "quit") break;
//      }
//      else
//         s = "This is a string of tokens";
//
//      boost::regex re("\\s+");
//      boost::sregex_token_iterator i(s.begin(), s.end(), re, -1);
//      boost::sregex_token_iterator j;
//
//      unsigned count = 0;
//      while(i != j)
//      {
//         cout << *i++ << endl;
//         count++;
//      }
//      cout << "There were " << count << " tokens found." << endl;
//
//   }while(argc == 1);
//   return 0;
//}