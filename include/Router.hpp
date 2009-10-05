#ifndef ROUTER_HPP_
#define ROUTER_HPP_

#include <iostream>
#include <string>


//TODO: quizas habria que armar otra clase, una para los routers obtenidos y otra para los known
class Router
{
public:

	Router()
	{
	}

	Router(const std::string& bssid, const std::string& ssid, int signal, int channel, bool security, std::string knownIpAddress = "")
		: bssid_(bssid), ssid_(ssid), signal_(signal), channel_(channel), security_(security), knownIpAddress_(knownIpAddress)
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
	std::string knownIpAddress_;

	std::string protocol_; //TODO: 802.11B o 802.11G,... etc... investigar...

protected:



	friend class boost::serialization::access;

	//TODO: poner estos métodos fuera de la clase como friend...
	//TODO: usar variante NO-INTRUSIVA
	template < typename Archive >
	void save(Archive & ar, const unsigned int version) const
	{
		//TODO: cambiar los nombres en el xml
		ar	& BOOST_SERIALIZATION_NVP(bssid_)
			& BOOST_SERIALIZATION_NVP(ssid_)
			& BOOST_SERIALIZATION_NVP(signal_)
			& BOOST_SERIALIZATION_NVP(channel_)
			& BOOST_SERIALIZATION_NVP(security_)
			& BOOST_SERIALIZATION_NVP(knownIpAddress_)
			;
		//ar & make_nvp("id", id_);
	}

	template < typename Archive >
	void load(Archive & ar, const unsigned int version)
	{
		ar	& BOOST_SERIALIZATION_NVP(bssid_)
			& BOOST_SERIALIZATION_NVP(ssid_)
			& BOOST_SERIALIZATION_NVP(signal_)
			& BOOST_SERIALIZATION_NVP(channel_)
			& BOOST_SERIALIZATION_NVP(security_)
			& BOOST_SERIALIZATION_NVP(knownIpAddress_)
			;

		//isDefault_ = version < 2;
	}


	BOOST_SERIALIZATION_SPLIT_MEMBER()


};



//TODO: borrar
std::ostream &operator<<(std::ostream &stream, Router router)
{
	
	stream << router.bssid_ << " - " << router.ssid_ << " - " << router.signal_ << " dB" << std::endl;
	return stream;
}

#endif //ROUTER_HPP_