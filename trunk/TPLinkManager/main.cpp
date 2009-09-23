// http://boost-extension.redshoelace.com/docs/boost/extension/boost_extension/tutorials/tutorial02.html

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

#include "AbstractRouterManager.hpp"
#include "Router.hpp"

#include "base64.hpp"



#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


using namespace boost::extensions;

////typedef factory<AbstractRouterManager, int> FactoryType;
//typedef factory<AbstractRouterManager, void> FactoryType;
typedef factory<AbstractRouterManager> FactoryType;
typedef std::map<std::string, FactoryType> FactoryMap;


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




class TpLinkManager : public AbstractRouterManager
{
public:
	virtual void connect()
	{
		try
		{

			std::string serverIp = "192.168.0.254";

			boost::asio::io_service io_service;

			// Get a list of endpoints corresponding to the server name.
			tcp::resolver resolver(io_service);
			//tcp::resolver::query query(argv[1], "http");
			tcp::resolver::query query(serverIp, "http");
			tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
			tcp::resolver::iterator end;

			// Try each endpoint until we successfully establish a connection.
			tcp::socket socket(io_service);
			boost::system::error_code error = boost::asio::error::host_not_found;
			while (error && endpoint_iterator != end)
			{
				socket.close();
				socket.connect(*endpoint_iterator++, error);
			}
			if (error)
			{
				throw boost::system::system_error(error);
			}

			// Form the request. We specify the "Connection: close" header so that the
			// server will close the socket after transmitting the response. This will
			// allow us to treat all data up until the EOF as the content.
			boost::asio::streambuf request;
			std::ostream request_stream(&request);
			//request_stream << "GET " << argv[2] << " HTTP/1.0\r\n";
			//request_stream << "GET " << "/" << " HTTP/1.0\r\n";
			request_stream << "GET " << "/" << " HTTP/1.1\r\n";

			//request_stream << "Host: " << argv[1] << "\r\n";
			request_stream << "Host: " << serverIp << "\r\n";
			request_stream << "Accept: */*\r\n";
			request_stream << "Connection: close\r\n";
			//request_stream << "Authorization: Basic YWRtaW46MTIzNA==\r\n";

			std::string usrAndPwd = "admin:candombe";
			request_stream << "Authorization: Basic " << base64_encode(usrAndPwd) << "\r\n";

			
			request_stream << "\r\n";
			

			// Send the request.
			boost::asio::write(socket, request);

			// Read the response status line.
			boost::asio::streambuf response;
			boost::asio::read_until(socket, response, "\r\n");


			// Check that response is OK.
			std::istream response_stream(&response);
			std::string http_version;
			response_stream >> http_version;
			unsigned int status_code;
			response_stream >> status_code;
			std::string status_message;
			std::getline(response_stream, status_message);

			if (!response_stream || http_version.substr(0, 5) != "HTTP/")
			{
				std::cout << "Invalid response\n";
				//return 1;
				return; 
			}

			if (status_code != 200)
			{
				std::cout << "Response returned with status code " << status_code << "\n";
				//return 1;
				return; 
			}

			// Read the response headers, which are terminated by a blank line.
			boost::asio::read_until(socket, response, "\r\n\r\n");

			// Process the response headers.
			std::string header;
			while (std::getline(response_stream, header) && header != "\r")
			{
				std::cout << header << "\n";
			}
			
			std::cout << "\n";

			// Write whatever content we already have to output.
			if (response.size() > 0)
			{
				std::cout << &response;
			}

			// Read until EOF, writing data to output as we go.
			while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
			{
				std::cout << &response;
			}
			
			if (error != boost::asio::error::eof)
			{
				throw boost::system::system_error(error);
			}
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << "\n";
		}

		//return 0;
		return;
	}




	virtual void parse(const std::string& html)
	{
		std::string htmlFile = "Z:\\Development\\CPP\\lastigen-haustiere\\Debug\\tplink_survey.htm";
		std::string regExFile = "Z:\\Development\\CPP\\lastigen-haustiere\\Debug\\regexvalue.txt";
		std::string htmlText;
		std::string regExText;

		loadFile(htmlText, htmlFile);
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

	virtual std::vector<Router> getRouterList()
	{
		return routers_;
		//std::cop
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


	
	std::vector<Router> routers_;
};


BOOST_EXTENSION_TYPE_MAP_FUNCTION 
{
	FactoryMap& factories(types.get());
	factories["TpLinkManager_factory"].set<TpLinkManager>();
}
