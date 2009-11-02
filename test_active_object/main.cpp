//// http://ciaranm.wordpress.com/2008/06/11/implementing-active-objects-using-smart-pointers/
//
//#include <iostream>
//
//#include <boost/shared_ptr.hpp>
//#include <boost/thread.hpp>
//
//typedef boost::mutex Mutex;
//typedef boost::mutex::scoped_lock Lock;
//
//
//
//template <typename T>
//class ActiveObjectPtr
//{
//private:
//	T ptr_;
//	//std::tr1::shared_ptr<Mutex> mutex_;
//	boost::shared_ptr<Mutex> mutex_;
//	
//
//	class Deref
//	{
//	private:
//		const ActiveObjectPtr * ptr_;
//		//std::tr1::shared_ptr<Lock> lock_;
//		boost::shared_ptr<Lock> lock_;
//
//	public:
//			//: ptr_(p), lock_(make_shared_ptr(new Lock(*p->mutex_)))
//
//		Deref(const ActiveObjectPtr * p) 
//			: ptr_(p), lock_(new Lock(*p->mutex_))
//		{
//		}
//
//		const T& operator->() const
//		{
//			return ptr_->ptr_;
//		}
//	};
//
//	friend class Deref;
//
//public:
//	ActiveObjectPtr(const T& t) 
//		: ptr_(t), mutex_(new Mutex)
//	{
//	}
//
//	ActiveObjectPtr(const ActiveObjectPtr & other) 
//		: ptr_(other.ptr_), mutex_(other.mutex_)
//	{
//	}
//
//	~ActiveObjectPtr()
//	{
//	}
//
//	ActiveObjectPtr& operator= (const ActiveObjectPtr & other)
//	{
//		if (this != &other)
//		{
//			ptr_ = other.ptr_;
//			mutex_ = other.mutex_;
//		}
//		return *this;
//	}
//
//	Deref operator->() const
//	{
//		return Deref(this);
//	}
//
//
//};
//
//
//class A
//{
//public:
//	void method()
//	{
//		std::cout << "void method()" << std::endl;
//	}
//};
//
//int main()
//{
//	
//	//ActiveObjectPtr<A> ao(new A);
//
//	A a;
//	ActiveObjectPtr<A> ao(a);
//	//ao->method();
//	ao.operator ->();
//}
//
//
////------------------------------------------------------------
//
////// http://ciaranm.wordpress.com/2008/06/13/on-demand-loading-using-smart-pointers/
//
////template <typename T>
////class DeferredConstructionPtr
////{
////private:
////    mutable T ptr_;
////    std::tr1::function<T ()> _f;
////    mutable bool _done;
////
////public:
////    DeferredConstructionPtr(const std::tr1::function<T ()> & f) :
////        ptr_(),
////        _f(f),
////        _done(false)
////    {
////    }
////
////    DeferredConstructionPtr(const DeferredConstructionPtr & other) :
////        ptr_(other.ptr_),
////        _f(other._f),
////        _done(other._done)
////    {
////    }
////
////    DeferredConstructionPtr &
////    operator= (const DeferredConstructionPtr & other)
////    {
////        if (this != &other)
////        {
////            ptr_ = other.ptr_;
////            _f = other._f;
////            _done = other._done;
////        }
////        return *this;
////    }
////
////    T operator-> () const
////    {
////        if (! _done)
////        {
////            ptr_ = _f();
////            _done = true;
////        }
////
////        return ptr_;
////    }
////};
//
