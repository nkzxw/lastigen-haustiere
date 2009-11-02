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
		}

		boost::shared_ptr<T> operator->()
		{
			return p_;
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
		return LockProxy(p_, mutex_);
	}

	private:
		boost::shared_ptr<T> p_;
		boost::shared_ptr<boost::recursive_mutex> mutex_;
};


class A
{
public:
	void method()
	{
		std::cout << "void method()" << std::endl;
	}
};

int main()
{
	//LockingProxy<A> lp(new A);
	
	boost::shared_ptr<A> asp(new A);
	LockPointer<A> lp(asp);
	lp->method();
	
}