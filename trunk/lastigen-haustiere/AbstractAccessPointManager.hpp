// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

//TODO: pasar a proyecto aparte, independiente

#ifndef ABSTRACTACCESSPOINTMANAGER_HPP_
#define ABSTRACTACCESSPOINTMANAGER_HPP_

#include <vector>

#include "Router.hpp"


class AbstractAccessPointManager 
{
public:
	
	AbstractAccessPointManager()
	{
	}
	
	virtual ~AbstractAccessPointManager() 
	{
	}

	virtual void connect() = 0;
	virtual void parse(const std::string& html) = 0;
	virtual std::vector<Router> getRouterList() = 0;

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
	//int age_;

};

#endif //ABSTRACTACCESSPOINTMANAGER_HPP_