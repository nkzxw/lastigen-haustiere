// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

//TODO: pasar a proyecto aparte, independiente

#ifndef ABSTRACTAPMANAGER_HPP_
#define ABSTRACTAPMANAGER_HPP_

#include <vector>

#include "appsettings.hpp"
#include "Router.hpp"


class AbstractAPManager 
{
public:

	AbstractAPManager(const AccessPointInformation& information)
		: routerListObtained_(false), information_(information)
	{
	}
	
	//virtual ~AbstractAPManager() 
	//{
	//}

	//virtual void connect() = 0;
	//virtual void parse(const std::string& html) = 0;
	virtual std::vector<Router> getRouterList(bool refresh = false) = 0;
	virtual void useRouter(const Router& router) const = 0;

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
	AccessPointInformation information_;


};

#endif //ABSTRACTAPMANAGER_HPP_