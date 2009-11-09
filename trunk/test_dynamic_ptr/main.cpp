#include <iostream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "dynamic_ptr.hpp"

template <typename T>
dynamic_ptr< T, changer > get()
{
	//return dynamic_ptr<T, changer<T> >(ptr_.pointee_, this);
	//return dynamic_ptr<T, changer>(ptr_.pointee_, this);
	//return dynamic_ptr< T, changer >( this );
	return dynamic_ptr< T, changer >();
}

int main(int argc, char** argv)
{
	changer<custom_class> c;
	c.load();

	//dynamic_ptr< custom_class, changer > ptr = get<custom_class>();


	////change_ptr<custom_class> ptr = c.get();
	////dynamic_ptr<custom_class, changer<custom_class> > ptr = c.get();
	//dynamic_ptr<custom_class, changer > ptr = c.get();

	//std::cout << ptr->data_ << std::endl;
	//std::cout << c.ptr_->data_ << std::endl;

	//c.reload();
	//std::cout << ptr->data_ << std::endl;
	//std::cout << c.ptr_->data_ << std::endl;


	std::cin.sync();
	std::cin.get();
	return 0;

	//------------------------------------------------------------------------------------------------------------


	return 0;
}





