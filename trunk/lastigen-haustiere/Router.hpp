#ifndef ROUTER_HPP_
#define ROUTER_HPP_

class Router
{
public:
	Router(const std::string& bssid, const std::string& ssid, int signal, int channel, bool security)
		: bssid_(bssid), ssid_(ssid), signal_(signal), channel_(channel), security_(security)
	{
	}

public: //protected:	//TODO:
	std::string bssid_;
	std::string ssid_;
	int signal_;
	int channel_;
	bool security_;
};

#endif //ROUTER_HPP_