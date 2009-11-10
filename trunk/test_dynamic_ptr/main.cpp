#include <iostream>
//#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "dynamic_ptr.hpp"

typedef changer<custom_class> ch;

int main(int argc, char** argv)
{
	changer<custom_class> c;
	c.load();

	changer<custom_class> c2;
	c2.load();


	boost::dynamic_ptr< changer<custom_class> > ptr = c.get();
	boost::dynamic_ptr< changer<custom_class> > ptr2 = c2.get();
	boost::dynamic_ptr< changer<custom_class> > ptr3= ptr;
	ptr3 = ptr2;
	//boost::dynamic_ptr< ch > ptr = c.get();



	std::cout << ptr->data_ << std::endl;
	//std::cout << c.ptr_->data_ << std::endl;
	std::cout << c.get()->data_ << std::endl;
	

	c.reload();
	std::cout << ptr->data_ << std::endl;
	//std::cout << c.ptr_->data_ << std::endl;
	std::cout << c.get()->data_ << std::endl;

	std::cin.sync();
	std::cin.get();
	return 0;

	//------------------------------------------------------------------------------------------------------------


	return 0;
}




//
//template <class Base, class Arg> 
//struct rebind; 
//
//template <template <class> class Base, class T, class Arg> 
//struct rebind<Base<T>, Arg> 
//{ 
//	typedef Base<Arg> type; 
//}; 
//
//template <class In> 
//void foo() 
//{
//	// d is of type container<double>. 
//	typename rebind<In, double>::type d; 
//} 
//
//template <class T>
//struct container {}; 
//
//int main() 
//{ 
//	foo<container<int> >(); 
//} 