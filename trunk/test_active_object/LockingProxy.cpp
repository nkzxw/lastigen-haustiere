// Modern C++ Design: http://books.google.com.ar/books?id=aJ1av7UFBPwC&pg=PA184&lpg=PA184&dq=Alexandrescu+LockingProxy&source=bl&ots=YQfKXuTh2-&sig=veutvNuRE3wIH3tsOPrbsCF4P7Y&hl=es&ei=3fnuSsvdLM2Vtgfnkd06&sa=X&oi=book_result&ct=result&resnum=3&ved=0CBUQ6AEwAg#v=onepage&q=&f=false

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

//typedef boost::mutex Mutex;
//typedef boost::mutex::scoped_lock Lock;

//template <class T>
//class LockingProxy
//{
//public:
//   LockingProxy(T* pObj) 
//	   : pointee_ (pObj), lock_(new boost::mutex::scoped_lock(mutex_))
//   { 
//	   //pointee_->Lock(); 
//   }
//   ~LockingProxy()
//   { 
//	   //pointee_->Unlock(); 
//   }
//   
//   T* operator->() const
//   { 
//	   return pointee_; 
//   }
//
//private:
//	boost::shared_ptr<boost::mutex::scoped_lock> lock_;
//	//boost::shared_ptr<boost::mutex> mutex_;
//	boost::mutex mutex_;
//
//
//	LockingProxy& operator=(const LockingProxy&);
//	T* pointee_;
//};


//------------------------------------------------------------------------------------------------


// http://bytes.com/topic/c/answers/532766-lock-pointer-implementation

template <typename T>
class LockPointer
{
public:

	class LockProxy
	{
	public:
		LockProxy(boost::shared_ptr<T> p, boost::shared_ptr<boost::recursive_mutex> mutex)
			: p_(p), lock_(new boost::recursive_mutex::scoped_lock(*mutex))
		{
			std::cout << "LockProxy()" << std::endl;
		}

		boost::shared_ptr<T> operator->()
		{
			return p_;
		}

		~LockProxy()
		{
			std::cout << "~LockProxy()" << std::endl;
		}

	private:
		boost::shared_ptr<T> p_;
		boost::shared_ptr<boost::recursive_mutex::scoped_lock> lock_;
	};

	LockPointer(boost::shared_ptr<T> o)
		: p_(o), mutex_(new boost::recursive_mutex())
	{
	}

	LockProxy operator->()
	{
		//mutex_->lock();
		return LockProxy(p_, mutex_);
	}

    T * get() const // never throws
    {
        return p_.get();
    }

	private:
		boost::shared_ptr<T> p_;
		boost::shared_ptr<boost::recursive_mutex> mutex_;
};


class A
{
public:
	A(int waitTime)
		: waitTime_(waitTime)
	{}

	void method()
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(waitTime_));
		std::cout << "void method()" << std::endl;
	}

	int waitTime_;
};

template<class T> inline T * get_pointer(LockPointer<T> const & p)
{
    return p.get();
}

int main()
{
	//LockingProxy<A> lp(new A);
	
	boost::shared_ptr<A> asp(new A(10000));
	LockPointer<A> lp(asp);
	//lp->method();

	boost::shared_ptr<A> asp2(new A(15000));
	LockPointer<A> lp2(asp2);
	//lp2->method();
	
	boost::thread thr1( boost::bind( &A::method, lp ) );
	boost::thread thr2( boost::bind( &A::method, lp2 ) );

	//thr1.join();
	//thr2.join();

	std::cin.get();
	return 0;
	
}