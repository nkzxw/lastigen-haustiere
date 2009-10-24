#ifndef SMARTNETWORKMANAGER_HPP_
#define SMARTNETWORKMANAGER_HPP_

//#include <boost/ptr_container/ptr_vector.hpp>
//#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <boost/thread.hpp>

#include "APManager.hpp"
#include "TpLinkController.hpp"



//bool SecurityDisabled( Router router )
//{ 
//	return (router.security_ == false);
//}



/*
// http://ubuntuforums.org/showthread.php?t=201776

class X {
public: 
   void run();
   void complexOperation( int limit );
};
...
X x;
boost::thread thr1( boost::bind( &X::run, &x ) );
boost::thread thr2( boost::bind( &X::complexOperation, &x, 30 ) );
boost::thread thr3( boost::bind( &X::complexOperation, &x, 100 ) );
... // while threads work in background
thr1.join();
thr2.join();
thr3.join();
*/



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

//typedef boost::scoped_ptr<AbstractAPController> APPtr;


void printRouterList(const AbstractAPController *controller)
{
	std::vector<Router> routers = controller->getRouterList();
	std::vector<Router> routersWithoutSecurity;

	//SecurityDisabled<Router> sd;
	//std::remove_copy_if (routers.begin(), routers.end(), std::back_inserter(routersWithoutSecurity), std::not1(SecurityDisabled<Router>()) );
	std::remove_copy_if (routers.begin(), routers.end(), std::back_inserter(routersWithoutSecurity), SecurityDisabled<Router>() );
	std::sort( routersWithoutSecurity.begin( ), routersWithoutSecurity.end( ), SignalGreater );

	//using namespace boost::lambda;
	////std::remove_copy_if(s.begin(), s.end(), std::back_inserter(t), _1 == '-');
	//std::remove_copy_if (routers.begin(), routers.end(), std::back_inserter(routersWithoutSecurity), _1 == false );

	std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
	std::copy (routersWithoutSecurity.begin(), routersWithoutSecurity.end(), std::ostream_iterator<Router>(std::cout));
}

class SmartNetworkManager
{
public:
	//typedef boost::ptr_unordered_map<std::string, AbstractAPController> MapType;
	typedef boost::ptr_unordered_map<std::string, APManager> APManagerListType;

	//SmartNetworkManager(const AppSettings& settings)
	SmartNetworkManager()
	{
		AppSettings *settings = ConfigManager<AppSettings>::instance->getSettings();

		//TODO: crear un typedef para std::vector<APInformation>
		for (std::vector<APInformation>::const_iterator it = settings->accessPoints_.begin(); it != settings->accessPoints_.end(); ++it)
		{
			//std::cout << it->name_ << std::endl;
			//std::cout << it->accessPointController_ << std::endl;

			APManager *tempAPManager = new APManager(*it);

			std::string tempStr = it->name_;			//TODO: ver como se puede hacer esta insercion más simple
			//accessPointControllers_.insert(tempStr, temp);
			apManagers_.insert(tempStr, tempAPManager);

		}

		//for (MapType::iterator it = accessPointControllers_.begin(); it != accessPointControllers_.end(); ++it)
		for (APManagerListType::const_iterator it = apManagers_.begin(); it != apManagers_.end(); ++it)
		{
			std::cout << it->first << std::endl;
			//std::cout << it->second->getName() << std::endl;
			////std::cout << it->second->name_ << std::endl;
			////std::cout << *it << std::endl;

			//boost::thread tempThread(  boost::bind( &AbstractAPController::getStatus, it->second ) );
			//tempThread.join();

			////TpLinkController cont;
			////boost::thread tempThread(  boost::bind( &TpLinkController::getStatus, &cont ) );
			////tempThread.join();


			//while ( it->second->getStatus() == APStatus::Disconnected )
			//{
			//	std::cout << "Connecting..." << std::endl;
			//}

			//it->second->refreshRouterList();
			//printRouterList(it->second);
		}





		//accessPointControllers_.insert(const_cast<std::string>(it->name_), temp);

		//for (int i = 0; i < 100; ++i)
		//{
		//	printRouterList(controller.get());

		//	Sleep(2000);
		//}

		
		//AbstractAPController *tempAPM; // = accessPointControllers_.at(0);

		////printRouterList(controller.get());
		//printRouterList(tempAPM);


		//int selection;

		//std::cout << "Elija el Router a utilizar: ";
		//std::cin >> selection;


		//std::cout << "El router seleccionado es: " << controller->getRouterList()[selection].bssid_ << std::endl;

		//controller->connectTo(manager->getRouterList()[selection]);


		

	}

protected:
	//MapType accessPointControllers_;
	APManagerListType apManagers_;

};

#endif // SMARTNETWORKMANAGER_HPP_

