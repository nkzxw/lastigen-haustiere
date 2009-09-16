// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

#ifndef ABSTRACTROUTERMANAGER_HPP_
#define ABSTRACTROUTERMANAGER_HPP_

class AbstractRouterManager 
{
public:
	
	AbstractRouterManager()
	{
	}
	
	virtual ~AbstractRouterManager() 
	{
	}

	virtual void parse(const std::string& html) = 0;
	virtual void getList() = 0;

	//virtual std::string get_name() 
	//{
	//	return "A generic AbstractParser";
	//}
	//
	//int get_age() 
	//{
	//	return age_;
	//}

protected:
	//int age_;

};

#endif //ABSTRACTROUTERMANAGER_HPP_