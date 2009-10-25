// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html
// http://www.boost.org/doc/libs/1_38_0/doc/html/xpressive/user_s_guide.html#boost_xpressive.user_s_guide.quick_start

#ifndef TPLINKCONTROLLER_HPP_
#define TPLINKCONTROLLER_HPP_

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>		// trim_copy, split, erase_all_copy
#include <boost/lexical_cast.hpp>
//#include <boost/regex.hpp>
#include <boost/xpressive/xpressive.hpp>


#include <boost/extension/extension.hpp>
#include <boost/extension/factory.hpp>
#include <boost/extension/type_map.hpp>

#include "AbstractAPController.hpp"
#include "Router.hpp"

#include "base64.hpp"
#include "HttpClient.hpp"


using boost::asio::ip::tcp;
using namespace boost::xpressive;


//#define STATUS_PAGE_REGEX "^(?:.*\\r?\\n)*var wlanPara = new Array\\(\\r?\\n\\d{0,4},\\r?\\n\"(?P<ssid>(?:\\w+))\",\\r?\\n(?P<channel>(?:\\d{0,4})),\\r?\\n\\d{0,4},\\r?\\n\"[\\w-]+\",\\r?\\n\"[\\w\\.]+\",\\r?\\n\\d{0,4},\\r?\\n\\d{0,4},\\r?\\n\"(?P<signal>(?:\\d{0,4})) dB\",\\r?\\n\\d{0,4},\\d{0,4} \\);(?:.*\\r?\\n)*$"

void loadFile(std::string& str, std::istream& inputStream)
{
   str.erase();
   // attempt to grow string buffer to match file size, this doesn't always work...
   str.reserve(inputStream.rdbuf()->in_avail());
   char c;
   while(inputStream.get(c))
   {
      // use logarithmic growth stategy, in case in_avail (above) returned zero:
      if(str.capacity() == str.size())
	  {
         str.reserve(str.capacity() * 3);
	  }
      str.append(1, c);
   }
}

void loadFile(std::string& str, const std::string& fileName)
{
	str.erase();
	std::ifstream inputStream(fileName.c_str());
	loadFile(str, inputStream);
}


//TODO: establecer un proceso automatico de refresco de la lista de routers... es necesario?????? En APManager

class TpLinkController : public AbstractAPController
{
public:

	//TODO: ver como hacer esto en el constructor. El problema está en la clase ReflectionManager
	//virtual void initialize(const APInformation& information) //, bool refresh = false
	//{
	//	this->information_ = information;

	//	//if (refresh)
	//	//{
	//	//	this->refreshRouterList();
	//	//}
	//}

	//TODO: ver de retornar el vector de forma eficiente para evitar la copia... Pasar por referencia o bien usar un shared_ptr<vector de punteros>
	virtual std::vector<Router> getRouterList() const		//bool refresh = false
	{
		std::vector<Router> tempRouters;

		//routers_.clear();
		HttpClient client;

		/*
		http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid			
		*/

		//std::string uriStr = "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid";
		//std::string uriStr = this->information_.routerListUri_;

		net::Uri uri(this->information_.protocol_, this->information_.host_, this->routerListQuery);

		//client.addHeader("Host", "www.google.com");
		//client.addHeader("Accept", "*/*");
		//client.addHeader("Connection", "close");

		if (information_.httpBasicCredentials_.size() > 0)
		{
			//std::string usrAndPwd = "admin:candombe";	//TODO: 
			std::string usrAndPwd = this->information_.httpBasicCredentials_;
			std::string credentials = base64_encode(usrAndPwd);
			client.addHeader("Authorization", "Basic " + credentials);
			//client.addHeader("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");
		}

		//std::string html = client.openRead(uriStr);
		std::string html = client.openRead(uri);
		this->parseRouterList(html, tempRouters);

		//return this->routers_;

		return tempRouters;

	}

	//virtual void refreshRouterList()
	//{
	//	//if (refresh || !routerListObtained_)
	//	//{
	//		routers_.clear();
	//		HttpClient client;

	//		/*
	//		http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid			
	//		*/

	//		//std::string uriStr = "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid";
	//		//std::string uriStr = this->information_.routerListUri_;

	//		net::Uri uri(this->information_.protocol_, this->information_.host_, this->routerListQuery);

	//		//client.addHeader("Host", "www.google.com");
	//		//client.addHeader("Accept", "*/*");
	//		//client.addHeader("Connection", "close");

	//		if (information_.httpBasicCredentials_.size() > 0)
	//		{
	//			//std::string usrAndPwd = "admin:candombe";	//TODO: 
	//			std::string usrAndPwd = this->information_.httpBasicCredentials_;
	//			std::string credentials = base64_encode(usrAndPwd);
	//			client.addHeader("Authorization", "Basic " + credentials);
	//			//client.addHeader("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");
	//		}

	//		//std::string html = client.openRead(uriStr);
	//		std::string html = client.openRead(uri);
	//		this->parseRouterList(html);

	//		//routerListObtained_ = true;
	//	//}

	//}


	virtual void connectTo(const Router& router) const
	{
		HttpClient client;

		//std::string uriStr = this->information_.useRouterUri_;
		std::string query = connectToQueryFirst + router.bssid_ + connectToQuerySecond;
		net::Uri uri(this->information_.protocol_, this->information_.host_, query);


		//client.addHeader("Host", "www.google.com");
		//client.addHeader("Accept", "*/*");
		//client.addHeader("Connection", "close");
		//std::string usrAndPwd = "admin:candombe";	//TODO: 
		std::string usrAndPwd = this->information_.httpBasicCredentials_;
		std::string credentials = base64_encode(usrAndPwd);
		client.addHeader("Authorization", "Basic " + credentials);
		//client.addHeader("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");

		//std::string html = client.openRead(uriStr);
		std::string html = client.openRead(uri);
		std::cout << html << std::endl;
	}

	virtual APStatus getStatus() const
	{
		//HttpClient client;

		//net::Uri uri(this->information_.protocol_, this->information_.host_, this->statusQuery);

		////client.addHeader("Host", "www.google.com");
		////client.addHeader("Accept", "*/*");
		////client.addHeader("Connection", "close");

		//if (information_.httpBasicCredentials_.size() > 0)
		//{
		//	//std::string usrAndPwd = "admin:candombe";	//TODO: 
		//	std::string usrAndPwd = this->information_.httpBasicCredentials_;
		//	std::string credentials = base64_encode(usrAndPwd);
		//	client.addHeader("Authorization", "Basic " + credentials);
		//	//client.addHeader("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");
		//}

		//std::string html = client.openRead(uri);

		std::string htmlFile = "Z:\\Development\\CPP\\lastigen-haustiere\\Debug\\tplink-status.html";
		std::string htmlText;
		loadFile(htmlText, htmlFile);

		//this->parseStatusPage(htmlText);

		//--------------------------------------------------------------------------------------------------------

		//std::cout << STATUS_PAGE_REGEX << std::endl;


		std::string ssid = "";
		std::string channel = "";
		std::string signal = "";

		smatch what;
		
		if(regex_match(htmlText, what, statusPageRegEx))
		{
			ssid = what[1];
			channel = what[2];
			signal = what[3];
		}

		if (ssid.size() > 0)
		{
			return APStatus::Connected;
		}
		else
		{
			return APStatus::Disconnected;
		}
	}
	
	virtual Router getConnectedRouter() const
	{
		//TODO:
		return ( Router() );
	}

protected:
	virtual void parseItem(const std::string& text, std::vector<Router>& routers) const
	{
		//typedef std::vector<std::string> split_vector_type;
		
		//TODO: usar boost::xpressive y Static RegEx 
		std::string regExText = "\"[^\"]*\"|\\d{1,}";

		boost::regex regularExpression(regExText, boost::regex::normal | boost::regbase::icase);

		boost::sregex_token_iterator it(text.begin(), text.end(), regularExpression);
		boost::sregex_token_iterator end;


		std::string bssid;
		std::string ssid;
		int signal;
		int channel;
		bool security;


		enum States {ExtractBSSID, ExtractSSID, ExtractSignal, ExtractChannel, ExtractSecurity};
		States state = ExtractBSSID;

		while(it != end)
		{
			std::string strItem = *it++;
			//std::cout << strItem << std::endl;

			switch (state)
			{
				case ExtractBSSID:
					//std::string bssid = boost::erase_all_copy(boost::trim_copy(*it++), "\"");
					bssid = boost::erase_all_copy(strItem, "\"");
					state = ExtractSSID;
					break;
				case ExtractSSID:
					ssid = boost::erase_all_copy(strItem, "\"");
					state = ExtractSignal;
					break;
				case ExtractSignal:

					try
					{
						signal = boost::lexical_cast<int>( boost::erase_all_copy( boost::erase_all_copy( strItem, "\"" ), " dB" ) );
					}
					catch(boost::bad_lexical_cast &)
					{
						//std::cout << "temp";
						//args.push_back(0);
					}
					state = ExtractChannel;
					break;
				case ExtractChannel:

					try
					{
						channel = boost::lexical_cast<int>(strItem);
					}
					catch(boost::bad_lexical_cast &)
					{
						//std::cout << "temp";
						//args.push_back(0);
					}

					state = ExtractSecurity;
					break;
				case ExtractSecurity:

					try
					{
						int tempSecurity = boost::lexical_cast<int>(strItem);
						security = (tempSecurity == 0);
					}
					catch(boost::bad_lexical_cast &)
					{
						//std::cout << "temp";
						//args.push_back(0);
					}

					//state = ExtractSSID;
					break;
			}
		}

		//routers_.push_back(Router(bssid, ssid, signal, channel, security));
		routers.push_back(Router(bssid, ssid, signal, channel, security));
 	    
		//std::vector<std::string> splitVec; 
		//boost::split( splitVec, text, boost::is_any_of(",") ); 
	}

	virtual void parseRouterList(const std::string& htmlText, std::vector<Router>& routers) const
	{

		//TODO: esta expresion regular, agregarla... o bien, como Xpressive->Static o bien al archivo de configuracion general.

		//////std::string htmlFile = "Z:\\Development\\CPP\\lastigen-haustiere\\Debug\\tplink_survey.htm";
		//////std::string regExFile = "Z:\\Development\\CPP\\lastigen-haustiere\\Debug\\regexvalue.txt";
		////std::string htmlFile = "E:\\Development\\CPP\\lastigen-haustiere\\Debug\\tplink_survey.htm";
		////std::string htmlText;


		//std::string regExFile = "E:\\Development\\CPP\\lastigen-haustiere\\Debug\\regexvalue.txt";
		//std::string regExText;
		//loadFile(regExText, regExFile);	// "[^"]*", "[^"]*", "[^"]*", \d{1,}, \d{1,}, 

		//--------------------------------------------------------------------------------------

		std::string regExText = "\"[^\"]*\", \"[^\"]*\", \"[^\"]*\", \\d{1,}, \\d{1,}, ";
                             //   "[^"]*", "[^"]*", "[^"]*", \d{1,}, \d{1,}, 
                             //  \"[^\"]*\", \"[^\"]*\", \"[^\"]*\", \d{1,}, \d{1,}, 

		//loadFile(htmlText, htmlFile);

		boost::regex regularExpression(regExText, boost::regex::normal | boost::regbase::icase);

		boost::sregex_token_iterator it(htmlText.begin(), htmlText.end(), regularExpression);
		boost::sregex_token_iterator end;

		while(it != end)
		{
			std::string strItem = *it++;
			//std::cout << strItem << std::endl;

			parseItem(strItem, routers);
		}

		//--------------------------------------------------------------------------------------

		//std::string regExText = "\"[^\"]*\", \"[^\"]*\", \"[^\"]*\", \\d{1,}, \\d{1,}, ";
		//                     //	   "[^"]*", "[^"]*", "[^"]*", \d{1,}, \d{1,}, 
		//sregex regularExpression = '"' >> *~'"' >> '"';

		////loadFile(htmlText, htmlFile);


		//boost::sregex_token_iterator it(htmlText.begin(), htmlText.end(), regularExpression);
		//boost::sregex_token_iterator end;

		//while(it != end)
		//{
		//	std::string strItem = *it++;
		//	//std::cout << strItem << std::endl;

		//	parseItem(strItem);
		//}
	}




	//std::vector<Router> routers_;

	static const std::string routerListQuery;       // "/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid";
	static const std::string connectToQueryFirst;   // "/userRpm/WlanModeRpm.htm?staSsid=&staType=1&staBssid=&rptBssid=&apMode=4&urptBssid="
	static const std::string connectToQuerySecond;  // "&pptBssid=&mptBssid1=&mptBssid2=&mptBssid3=&mptBssid4=&mptBssid5=&mptBssid6=&Save=Save"
	static const std::string statusQuery;			 // /userRpm/StatusRpm.htm

	static const sregex commonDigitRegEx;
	static const sregex statusPageRegEx;

};

//TODO: ver si estos valores van a ir harcodeados en el binario o bien en una seccion aparte del archivo de configuracion...
const std::string TpLinkController::routerListQuery = "/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid";
const std::string TpLinkController::connectToQueryFirst = "/userRpm/WlanModeRpm.htm?staSsid=&staType=1&staBssid=&rptBssid=&apMode=4&urptBssid=";
const std::string TpLinkController::connectToQuerySecond = "&pptBssid=&mptBssid1=&mptBssid2=&mptBssid3=&mptBssid4=&mptBssid5=&mptBssid6=&Save=Save";
const std::string TpLinkController::statusQuery = "/userRpm/StatusRpm.htm";


// Compile-time regex
//const sregex TpLinkController::statusPageRegEx = sregex::compile(STATUS_PAGE_REGEX);
const sregex TpLinkController::commonDigitRegEx = repeat<0,4>(_d);
const sregex TpLinkController::statusPageRegEx = bos >> *_ >> "var wlanPara = new Array(" >> _ln >> _d >> commonDigitRegEx >> ',' >> _ln >> '"' >> (s1= +_w) >> "\"," >> _ln >> (s2= commonDigitRegEx) >> ',' >> _ln >> commonDigitRegEx >> ',' >> _ln >> '"' >> +(_w | '-') >> "\"," >> _ln >> '"' >> +(_w | '.') >> "\"," >> _ln >> commonDigitRegEx >> ',' >> _ln >> commonDigitRegEx >> ',' >> _ln >> '"' >> (s3= commonDigitRegEx) >> " dB\"," >> _ln >> commonDigitRegEx >> ',' >> commonDigitRegEx >> " );" >> *_ >> eos;


#endif //TPLINKCONTROLLER_HPP_

