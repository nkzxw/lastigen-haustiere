#ifndef THREADMANAGER_HPP_
#define THREADMANAGER_HPP_

#include <vector>

#include <boost/thread.hpp>


template <typename T>
class ThreadedClass
{
public:
//        : stopRequested_(false), thread_(boost::bind(&ThreadedClass::doWork, this)) //Note 2
//        : stopRequested_(false), thread_(boost::bind( &T::doWork, this)) //Note 2

	ThreadedClass()
        : stopRequested_(false)//, thread_(boost::bind( &T::doWork, dynamic_cast<T*>(this) )) //Note 2
    {
		//this->doWork();
		//dynamic_cast<T*>(this)->doWork();
    }
	
	virtual void test()
	{
		this->doWork();
		//dynamic_cast<T*>(this)->doWork();
		static_cast<T*>(this)->doWork();
	}
 
    ~ThreadedClass()
    {
        stopRequested_ = true;
        thread_.join(); //Note 2
    }
 
	virtual void doWork() = 0;

protected:
    volatile bool stopRequested_;
    boost::thread thread_;
 
};



//class ThreadedClass
//{
//public:
//    ThreadedClass()
//        : stopRequested_(false),  
//          thread_(boost::bind(&ThreadedClass::doWork, this)) //Note 2
//    {
//    }
// 
//    ~ThreadedClass()
//    {
//        stopRequested_ = true;
//        thread_.join(); //Note 2
//    }
// 
//    int getFibonacciValue(int which)
//    {
//        boost::mutex::scoped_lock l(mutex_);
//        return fibonacciValues_.get(which);
//    }
// 
//private:
//    volatile bool stopRequested_;
//    std::vector<int> fibonacciValues_;
//    boost::mutex mutex_;
//    boost::thread thread_;
// 
//    int fibonacciNumber(int num)
//    {
//        switch(num)
//        {
//            case 0:
//            case 1:
//                return 1;
//            default:
//                return fib(num-2) + fib(num-1);
//        };
//    }    
// 
//    // Compute and save fibonacci numbers as fast as possible
//    void doWork()
//    {
//        int iteration = 0;
//        while (!stopRequested_)
//        {
//            int value = fibonacciNumber(iteration);
//            boost::mutex::scoped_lock l(mutex_);
//            fibonacciValues_.push_back(value);
//        }
//    }                    
//};

#endif // THREADMANAGER_HPP_