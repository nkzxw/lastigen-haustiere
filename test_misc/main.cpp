#include <iostream>
#include <boost/utility.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>

void sender() 
{
	boost::this_thread::sleep(boost::posix_time::milliseconds(20000));
}

void receiver() 
{

	boost::this_thread::sleep(boost::posix_time::milliseconds(15000));
}

int main(int, char*[])
{
    boost::thread thrd1(&sender);
    boost::thread thrd2(&receiver);

	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

	thrd1.interrupt();
	thrd2.interrupt();

    thrd1.join();
    thrd2.join();
    return 0;
}

// -----------------------------------------------------------------------------------------------------------------------


//#include <iostream>
//#include <vector>
//#include <boost/utility.hpp>
//#include <boost/thread/condition.hpp>
//#include <boost/thread/thread.hpp>
//
//class bounded_buffer : private boost::noncopyable
//{
//public:
//    typedef boost::mutex::scoped_lock lock;
//
//    bounded_buffer(int n) : begin(0), end(0), buffered(0), circular_buf(n) { }
//
//    void send (int m) 
//	{
//        lock lk(monitor);
//        while (buffered == circular_buf.size())
//		{
//            buffer_not_full.wait(lk);
//		}
//        circular_buf[end] = m;
//        end = (end+1) % circular_buf.size();
//        ++buffered;
//        buffer_not_empty.notify_one();
//    }
//
//    int receive() 
//	{
//        lock lk(monitor);
//        while (buffered == 0)
//		{
//            buffer_not_empty.wait(lk);
//		}
//        int i = circular_buf[begin];
//        begin = (begin+1) % circular_buf.size();
//        --buffered;
//        buffer_not_full.notify_one();
//        return i;
//    }
//
//private:
//    int begin, end, buffered;
//    std::vector<int> circular_buf;
//    boost::condition buffer_not_full, buffer_not_empty;
//    boost::mutex monitor;
//};
//
//bounded_buffer buf(2);
//
//void sender() 
//{
//    int n = 0;
//    while (n < 100) 
//	{
//        buf.send(n);
//        std::cout << "sent: " << n << std::endl;
//        ++n;
//    }
//    buf.send(-1);
//}
//
//void receiver() 
//{
//    int n;
//    do 
//	{
//        n = buf.receive();
//        std::cout << "received: " << n << std::endl;
//    } while (n != -1); // -1 indicates end of buffer
//}
//
//int main(int, char*[])
//{
//    boost::thread thrd1(&sender);
//    boost::thread thrd2(&receiver);
//    thrd1.join();
//    thrd2.join();
//    return 0;
//}


// -----------------------------------------------------------------------------------------------------------------------

//
////template <template <typename T> class X> 
////struct A 
////{
////	X<T> member2_;
////};
////
////template <typename T>
////struct B 
////{
////	T member_;
////};
////
////struct C
////{
////};
////
////
//////template<class _Kty, class _Ty, class _Pr = less<_Kty>, class _Alloc = allocator<pair<const _Kty, _Ty> > >
//////class map
//////template <class Key, class T, class Hash, class Pred, class Alloc>
//////class unordered_map
////
////
//int main()
//{
//
//	//B<int> b;
//	//b.member_ = 10;
//
//
//	//A<B> ab;
//
//	//
//	////A<C> ac;		//No funciona
//
//	
//	return 0;
//}