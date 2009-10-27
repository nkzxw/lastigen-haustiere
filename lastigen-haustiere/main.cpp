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


//#include <boost/utility/singleton.hpp> //torjo
#include <boost/utility/mutexed_singleton.hpp> //torjo

//class temp : public ThreadedClass<temp>
//{
//public:
//    int getFibonacciValue(int which)
//    {
//        boost::mutex::scoped_lock l(mutex_);
//        //return fibonacciValues_.get(which);
//		return fibonacciValues_[which];
//    }
//
//    virtual void doWork()
//    {
//        int iteration = 0;
//        while (!stopRequested_)
//        {
//            int value = fibonacciNumber(iteration);
//			
//			{
//				boost::mutex::scoped_lock l(mutex_);
//				fibonacciValues_.push_back(value);
//			}
//			++iteration;
//        }
//	}
// 
//protected:
//    boost::mutex mutex_;
//    std::vector<int> fibonacciValues_;
// 
//    int fibonacciNumber(int num)
//    {
//        switch(num)
//        {
//            case 0:
//            case 1:
//                return 1;
//            default:
//                return fibonacciNumber(num-2) + fibonacciNumber(num-1);
//        };
//
//		//return fib(num-2) + fib(num-1);
//    }    
// 
//    // Compute and save fibonacci numbers as fast as possible
//
//
//};
//

//TODO: ver si agrego una clase Application, que se encargue del ConfigManager y el Logging. Algo comun en casi todo tipo de aplicaciones. Ver de usar boost::program_options
int main(int argc, char** argv) 
{

	//{
	////temp t2;
	////boost::shared_ptr<temp> t = ThreadedClass::Create<temp>();
	//boost::shared_ptr<temp> t = ThreadedClass<temp>::Create();

	//boost::this_thread::sleep(boost::posix_time::milliseconds(1500));
	////boost::this_thread::sleep(boost::posix_time::nanoseconds(10)); 

	//int returnvalue = t->getFibonacciValue(2);
	//}
	


 //   ConfigManager<AppSettings>::lease cm;
	//cm->initialize( argv[0] );
	ConfigManager<AppSettings>::instance->initialize( argv[0] );
	//AppSettings *cfg = cm.getSettings();


	{
	SmartNetworkManager snm;

	std::cout << "press Ctrl-c to exit..." << std::endl;

	while (true) // check the internal state of the connection to make sure it's still running
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(300));

		char ch;
		cin.get(ch); // blocking wait for standard input
		if (ch == 3) // ctrl-C to end program
		{
			break;
		}
	}

	std::cout << "--- DESTRUCTORES ---" << std::endl;
	}

	std::cin.sync();
	std::cin.get();
	return 0;
}
