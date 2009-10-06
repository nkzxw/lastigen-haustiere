// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

//TODO: pasar a proyecto aparte, independiente

#ifndef ABSTRACTAPMANAGER_HPP_
#define ABSTRACTAPMANAGER_HPP_

#include <vector>

#include "AppSettings.hpp"
#include "Router.hpp"


enum APStatus
{
	Disconnected = 0,
	Connected = 1,
	NotWorking = 2
};

class AbstractAPManager 
{
public:

	//AbstractAPManager(const APInformation& information)
	//	: routerListObtained_(false), information_(information)
	//{
	//}

	AbstractAPManager()
		: routerListObtained_(false)
	{}

	virtual void initialize(const APInformation& information) = 0;

	//virtual ~AbstractAPManager() 
	//{
	//}

	virtual std::vector<Router> getRouterList(bool refresh = false) = 0;	//TODO: ver modo de refrescar los datos
	virtual void connectTo(const Router& router) const = 0;
	virtual APStatus getStatus() const = 0;				//TODO: ver modo de refrescar los datos
	
	//TODO: ver de retornar una referencia o puntero del Router
	virtual Router getConnectedRouter() const = 0;		//TODO: ver modo de refrescar los datos


	std::string getName() const
	{
		return information_.name_;
	}

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
	//std::string name_;
	bool routerListObtained_;
	APInformation information_;


};

#endif //ABSTRACTAPMANAGER_HPP_
