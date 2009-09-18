#ifndef ROUTER_HPP_
#define ROUTER_HPP_

#include <iostream>
#include <string>

class Router
{
public:

	Router()
	{
	}

	Router(const std::string& bssid, const std::string& ssid, int signal, int channel, bool security)
		: bssid_(bssid), ssid_(ssid), signal_(signal), channel_(channel), security_(security)
	{
	}

	friend std::ostream &operator<<(std::ostream &stream, Router ob);
	//friend std::istream &operator>>(istream &stream, MyClass &ob);


public: //protected:	//TODO:
	std::string bssid_;
	std::string ssid_;
	int signal_;
	int channel_;
	bool security_;
};



std::ostream &operator<<(std::ostream &stream, Router router)
{
	
	stream << router.bssid_ << " - " << router.ssid_ << " - " << router.signal_ << " dB" << std::endl;
	return stream;
}

#endif //ROUTER_HPP_