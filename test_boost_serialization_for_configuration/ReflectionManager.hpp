#ifndef REFLECTIONMANAGER_HPP
#define REFLECTIONMANAGER_HPP

#include <typeinfo>

#include <boost/function.hpp>
//#include <boost/lambda/lambda.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/extension/factory.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/extension/type_map.hpp>

#include <boost/unordered_map.hpp>


using namespace boost::extensions;

//#define DECLARE_SPELLABLE( a_type ) template<> class spelling<a_type> { public: static const std::string result; }; const std::string spelling<my_enum>::result = #a_type
//DECLARE_SPELLABLE(int);

class ReflectionManager
{
public:

	ReflectionManager()
	{
	}

	//TODO: excepciones
	template <typename T>
	static T* CreateInstance(const std::string& sharedLibraryPath, const std::string& dynamicType)
	{
		//std::string typeNameStr = typeid(T).name();


		//typedef std::map<std::string, boost::extensions::factory<T, void> > FactoryMap;
		typedef boost::extensions::factory<T, void> FactoryType;
		typedef std::map<std::string, FactoryType> FactoryMap;
	
		

		// Create shared_library object with the relative or absolute
		// path to the shared library.
		shared_library lib(sharedLibraryPath);


		// Attempt to open the shared library.
		if (!lib.open()) 
		{
			std::cerr << "Library failed to open: " << sharedLibraryPath << std::endl;
			return 0; //1;
		}

		return CreateInstance<T>(lib, dynamicType);




		
		//// Use the shared_library::call to auto matically call an Extension-specific function in the shared library, which takes a type_map.
		//type_map types;
		//if (!lib.call(types)) 
		//{
		//	std::cerr << "Function not found!" << std::endl;
		//	return 0; //1;
		//}

		//// Retrieve a map of all animal factories taking an int and indexed by a string from the type_map.
		//FactoryMap& factories(types.get());
		//if (factories.empty()) 
		//{
		//	std::cerr << "Animals not found!" << std::endl;
		//	return 0; //1;
		//}

		//std::string factoryName = dynamicType + "_factory";
		//if ( factories.find(factoryName) == factories.end() )		//TODO: ver si es la forma correcta de chequear que existe el key
		//{
		//	std::cerr << "Error in Shared Library. No Factory founded." << std::endl;
		//	return 0; //1;
		//}

		//return factories[factoryName].create();
	}




	
	//TODO: excepciones
	template <typename T>
	static T* CreateInstance(shared_library& lib, const std::string& dynamicType)
	{
		typedef boost::extensions::factory<T, void> FactoryType;
		typedef std::map<std::string, FactoryType> FactoryMap;

		
		// Use the shared_library::call to auto matically call an Extension-specific function in the shared library, which takes a type_map.
		type_map types;
		if (!lib.call(types)) 
		{
			std::cerr << "Function not found!" << std::endl;
			return 0; //1;
		}

		// Retrieve a map of all animal factories taking an int and indexed by a string from the type_map.
		FactoryMap& factories(types.get());
		if (factories.empty()) 
		{
			std::cerr << "Animals not found!" << std::endl;
			return 0; //1;
		}

		std::string factoryName = dynamicType + "_factory";
		if ( factories.find(factoryName) == factories.end() )		//TODO: ver si es la forma correcta de chequear que existe el key
		{
			std::cerr << "Error in Shared Library. No Factory founded." << std::endl;
			return 0; //1;
		}

		return factories[factoryName].create();
	}




protected:

};



//class ReflectionManager
//{
//public:
//
//	//typedef std::map<std::string, std::string> MappingType;
//	typedef boost::unordered_map<std::string, std::string> MappingType;
//
//	ReflectionManager(const MappingType& mappings)
//		: mappings_(mappings)
//	{
//	}
//
//	template <typename T>
//	T* get(const std::string& dynamicType)
//	{
//		//std::string typeNameStr = typeid(T).name();
//
//		typedef factory<AbstractAccessPointManager, void> FactoryType;
//		typedef std::map<std::string, FactoryType> FactoryMap;
//
//
//		
//		//std::string library_path = "TPLinkManager.dll";
//		std::string library_path = mappings_[dynamicType];
//
//		// Create shared_library object with the relative or absolute
//		// path to the shared library.
//		shared_library lib(library_path);
//
//		// Attempt to open the shared library.
//		if (!lib.open()) 
//		{
//			std::cerr << "Library failed to open: " << library_path << std::endl;
//			return 0; //1;
//		}
//
//		// Use the shared_library::call to automatically call an Extension-specific function in the shared library,
//		// which takes a type_map.
//		type_map types;
//		if (!lib.call(types)) 
//		{
//			std::cerr << "Function not found!" << std::endl;
//			return 0; //1;
//		}
//
//		// Retrieve a map of all animal factories taking an int and indexed by a string from the type_map.
//		FactoryMap& factories(types.get());
//		if (factories.empty()) 
//		{
//			std::cerr << "Animals not found!" << std::endl;
//			return 0; //1;
//		}
//
//		//TpLinkManager_factory
//		std::string factoryName = dynamicType + "_factory";
//		if ( factories.find(factoryName) == factories.end() )								//TODO: ver si es la forma correcta de chequear que existe el key
//		{
//			std::cerr << "Error in Shared Library. No Factory founded." << std::endl;
//			return 0; //1;
//		}
//
//		//boost::scoped_ptr<AbstractAccessPointManager> type(factories["factory"].create());
//		//return type;
//
//		return factories[factoryName].create();
//	}
//
//protected:
//	MappingType mappings_;
//};

#endif // REFLECTIONMANAGER_HPP
