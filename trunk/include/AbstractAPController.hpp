// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

//TODO: pasar a proyecto aparte, independiente

#ifndef ABSTRACTAPCONTROLLER_HPP_
#define ABSTRACTAPCONTROLLER_HPP_

#include <vector>

#include "AppSettings.hpp"
#include "Router.hpp"


enum APConnectionStatus
{
	Disconnected = 0,
	Connected = 1,
	NotWorking = 2
};

class APStatus
{
public:
	APConnectionStatus connectionStatus_;
	Router connectedRouter_;

};

class AbstractAPController
{
public:

	//AbstractAPController(const APInformation& information)
	//	: routerListObtained_(false), information_(information)
	//{
	//}

	AbstractAPController()
		//: routerListObtained_(false)
	{}

	//virtual void initialize(const APInformation& information, bool refresh = false) = 0;
	virtual void initialize(const APInformation& information)
	{
		this->information_ = information;
	}


	//virtual ~AbstractAPController() 
	//{
	//}

	//virtual std::vector<Router> getRouterList(bool refresh = false) const = 0;
	virtual std::vector<Router> getRouterList() const = 0;
	virtual void connectTo(const Router& router) const = 0;
	//virtual APConnectionStatus getStatus() const = 0;
	virtual const boost::shared_ptr<Router> getStatus() const = 0;
	//virtual Router* getStatus() const = 0;

	//TODO: ver de retornar una referencia o puntero del Router
	virtual Router getConnectedRouter() const = 0;


	//TODO: ver de retornar una referencia o puntero
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
	//bool routerListObtained_;
	APInformation information_;


};

#endif //ABSTRACTAPCONTROLLER_HPP_
