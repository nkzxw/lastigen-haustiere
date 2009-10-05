// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

//TODO: pasar a proyecto aparte, independiente

#ifndef AbstractAPManager_HPP_
#define AbstractAPManager_HPP_

#include <vector>

#include "Router.hpp"


//TODO: cambiarle el nombre a AbstractAPManager
class AbstractAPManager 
{
public:
	
	AbstractAPManager()
		: routerListObtained_(false)
	{
	}
	
	virtual ~AbstractAPManager() 
	{
	}

	virtual void connect() = 0;
	//virtual void parse(const std::string& html) = 0;
	virtual std::vector<Router> getRouterList(bool refresh = false) = 0;

	//virtual std::string get_name() 
	//{
	//	return "A generic AbstractParser";
	//}
	//
	//int get_age() 
	//{
	//	return age_;
	//}

protected:
	bool routerListObtained_;

};

#endif //AbstractAPManager_HPP_