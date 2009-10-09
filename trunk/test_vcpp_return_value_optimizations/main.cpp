// http://msdn.microsoft.com/en-us/library/ms364057(VS.80).aspx

#include <iostream>
#include <vector>


struct A
{
	A()
	{
		std::cout << "A()" << std::endl;
	}

	~A()
	{
		std::cout << "~A()" << std::endl;
	}

	A(const A& a)
	{
		std::cout << "A(const A& a)" << std::endl;
	}
};

std::vector<A> func()
{
	std::vector<A> temp;
	temp.push_back(A());

	std::cout << "----------------------------------------------------------------" << std::endl;

	return temp;
}

int main()
{
	//{
	//	A a;
	//	A a2 = a;
	//}

	std::cout << "----------------------------------------------------------------" << std::endl;

	//std::vector<A> AVector;
	//AVector.push_back(A());

	
	std::cout << "----------------------------------------------------------------" << std::endl;

	std::vector<A> AVector = func();

	std::cin.get();
	return 0;

}
