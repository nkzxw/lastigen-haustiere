// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

class AbstractParser 
{
public:
	
	AbstractParser()
	{
	}
	
	virtual ~AbstractParser() 
	{
	}

	virtual std::string get_name() 
	{
		return "A generic AbstractParser";
	}
	
	int get_age() 
	{
		return age_;
	}

protected:
	//int age_;

};
