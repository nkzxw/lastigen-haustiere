// http://blog.emptycrate.com/node/277
// http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Calling_Virtuals_During_Initialization


#ifndef THREADEDCLASS_HPP_INCLUDED
#define THREADEDCLASS_HPP_INCLUDED

//#include <vector>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>


template <typename T>
class ThreadedClass
{
public:
	virtual void init()
	{
		//static_cast<T*>(this)->doWork();

        assert(!thread_);
        thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&ThreadedClass::doWork, this)));

	}
	
    static boost::shared_ptr<T> Create()
    {
       boost::shared_ptr<T> ptr (new T);
       ptr->init (); 
       return ptr;
    }

    template <typename P1>
    static boost::shared_ptr<T> Create (P1 p1)
    {
       boost::shared_ptr<T> ptr (new T(p1));
       ptr->init (); 
       return ptr;
    }

	//TODO: ver como hacer para emular "variadic templates parameters", o sea, multiples parametros de templates, pero sin hacer N métodos similares. 
	template <typename P1, typename P2>
    static boost::shared_ptr<T> Create (P1 p1, P2 p2)
    {
       boost::shared_ptr<T> ptr (new T(p1, p2));
       ptr->init (); 
       return ptr;
    }

    ~ThreadedClass()
    {
        stopRequested_ = true;
        thread_->join(); //Note 2
    }
 
	virtual void doWork() = 0;
	//TODO: implementar start, stop, interrupt, sleep, etc...

protected:
//        : stopRequested_(false), thread_(boost::bind(&ThreadedClass::doWork, this)) //Note 2
//        : stopRequested_(false), thread_(boost::bind( &T::doWork, this)) //Note 2

	ThreadedClass()
        : stopRequested_(false)//, thread_(boost::bind( &T::doWork, dynamic_cast<T*>(this) )) //Note 2
    {
		//this->doWork();
		//dynamic_cast<T*>(this)->doWork();
		//static_cast<T*>(this)->doWork();
    }

    volatile bool stopRequested_;
    //boost::thread thread_;
    boost::shared_ptr<boost::thread> thread_;


private:

};

#endif // THREADEDCLASS_HPP_INCLUDED