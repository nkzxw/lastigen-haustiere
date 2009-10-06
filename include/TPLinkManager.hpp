// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

#ifndef TPLINKMANAGER_HPP_
#define TPLINKMANAGER_HPP_

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>		// trim_copy, split, erase_all_copy
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>


#include <boost/extension/extension.hpp>
#include <boost/extension/factory.hpp>
#include <boost/extension/type_map.hpp>

#include "AbstractAPManager.hpp"
#include "Router.hpp"

#include "base64.hpp"
#include "HttpClient.hpp"





using boost::asio::ip::tcp;

//using namespace boost::extensions;



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




class TpLinkManager : public AbstractAPManager
{
public:

	//TODO: ver como hacer esto en el constructor. El problema est� en la clase ReflectionManager
	virtual void initialize(const APInformation& information)
	{
		this->information_ = information;
	}

	virtual std::vector<Router> getRouterList(bool refresh = false)
	{
		if (refresh || !routerListObtained_)
		{
			routers_.clear();
			HttpClient client;

			//std::string uriStr = "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid";
			std::string uriStr = this->information_.routerListUri_;

			//client.addHeader("Host", "www.google.com");
			//client.addHeader("Accept", "*/*");
			//client.addHeader("Connection", "close");
			//std::string usrAndPwd = "admin:candombe";	//TODO: 
			std::string usrAndPwd = this->information_.httpBasicCredentials_;
			std::string credentials = base64_encode(usrAndPwd);
			client.addHeader("Authorization", "Basic " + credentials);
			//client.addHeader("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");

			std::string html = client.openRead(uriStr);
			this->parseRouterList(html);

			routerListObtained_ = true;
		}

		return routers_;
	}


	virtual void connectTo(const Router& router) const
	{
		HttpClient client;

		std::string uriStr = this->information_.useRouterUri_;

		//client.addHeader("Host", "www.google.com");
		//client.addHeader("Accept", "*/*");
		//client.addHeader("Connection", "close");
		//std::string usrAndPwd = "admin:candombe";	//TODO: 
		std::string usrAndPwd = this->information_.httpBasicCredentials_;
		std::string credentials = base64_encode(usrAndPwd);
		client.addHeader("Authorization", "Basic " + credentials);
		//client.addHeader("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");

		std::string html = client.openRead(uriStr);
		std::cout << html << std::endl;
	}

	virtual bool isConnected() const
	{
		//TODO:
		return false;
	}
	
	virtual Router getConnectedRouter() const
	{
		//TODO:
		return ( Router() );
	}

protected:
	virtual void parseItem(const std::string& text)
	{
		//typedef std::vector<std::string> split_vector_type;
		
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

		routers_.push_back(Router(bssid, ssid, signal, channel, security));
 	    
		//std::vector<std::string> splitVec; 
		//boost::split( splitVec, text, boost::is_any_of(",") ); 
	}


	virtual void connect()
	{
		HttpClient client;

		std::string uriStr = "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid";
		client.addHeader("Host", "www.google.com");
		client.addHeader("Accept", "*/*");
		client.addHeader("Connection", "close");
		std::string usrAndPwd = "admin:candombe";	//TODO: 
		std::string credentials = base64_encode(usrAndPwd);
		client.addHeader("Authorization", "Basic " + credentials);
		//client.addHeader("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)");

		//TODO: de donde sacamos esta URL ????
		client.openRead(uriStr);
	}




	virtual void parseRouterList(const std::string& htmlText) 
	{
		////std::string htmlFile = "Z:\\Development\\CPP\\lastigen-haustiere\\Debug\\tplink_survey.htm";
		////std::string regExFile = "Z:\\Development\\CPP\\lastigen-haustiere\\Debug\\regexvalue.txt";
		//std::string htmlFile = "E:\\Development\\CPP\\lastigen-haustiere\\Debug\\tplink_survey.htm";
		std::string regExFile = "E:\\Development\\CPP\\lastigen-haustiere\\Debug\\regexvalue.txt";
		//std::string htmlText;
		std::string regExText;

		//loadFile(htmlText, htmlFile);
		loadFile(regExText, regExFile);	// "[^"]*", "[^"]*", "[^"]*", \d{1,}, \d{1,}, 

		boost::regex regularExpression(regExText, boost::regex::normal | boost::regbase::icase);

		boost::sregex_token_iterator it(htmlText.begin(), htmlText.end(), regularExpression);
		boost::sregex_token_iterator end;

		while(it != end)
		{
			std::string strItem = *it++;
			//std::cout << strItem << std::endl;

			parseItem(strItem);
		}
	}

	
	std::vector<Router> routers_;
};


#endif //TPLINKMANAGER_HPP_

