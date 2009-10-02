//TODO: pasar a proyecto aparte, independiente

#ifndef ABSTRACTIPCHANGER_HPP_
#define ABSTRACTIPCHANGER_HPP_

#include <string>

class AbstractIpChanger 
{
public:

	//TODO: ver si es necesario
	AbstractIpChanger()
	{
	}
	
	//TODO: ver si es necesario
	virtual ~AbstractIpChanger() 
	{
	}

	virtual bool setStatic(const std::string& nicName, const std::string& ipAddress, const std::string& subnetMask, const std::string& defaultGateway) = 0;
	virtual bool setDynamic(const std::string& nicName) = 0;
	virtual std::string getMessage() const = 0;

	//virtual std::string get_name() 
	//{
	//	return "A generic AbstractParser";
	//}

protected:
	std::string message_;
};

#endif //ABSTRACTIPCHANGER_HPP_