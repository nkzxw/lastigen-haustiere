#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <algorithm>
#include <iostream>
#include <istream>
//#include <map>
#include <ostream>
#include <string>
#include <sstream>
//#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/asio.hpp>
//#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/finder.hpp>
#include <boost/algorithm/string/regex.hpp>

#include "Uri.hpp"


using boost::asio::ip::tcp;

//TODO: namespaces
class HttpClient
{
public:

	//typedef std::map<std::string, std::string> HeaderMap;
	typedef boost::unordered_map<std::string, std::string> HeaderMap;


	//TODO: ver si se crea una clase aparte para los Headers.
	virtual void addHeader(const std::string& key, const std::string& value)
	{
		headers_[key] = value;
	}



	virtual std::string openRead(const std::string& uriStr)
	{
		//TODO: manejo de excepciones al crear la Uri
		//TODO: manejo de excepciones del openRead

		return openRead(net::Uri(uriStr));
	}

	//TODO: ver de hacer más performante, evitar la copia
	//TODO: manejo de excepciones
	virtual std::string openRead(const net::Uri& uri)
	{
		//TODO: solo debe soportar protocolos http o https... (por ahora solo http)

		try
		{
			boost::asio::io_service io_service;

			// Get a list of endpoints corresponding to the server name.
			tcp::resolver resolver(io_service);
			
			tcp::resolver::query query(uri.getHost(), uri.getProtocol());
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

			//TODO: el protocolo y la version podrian ir parametrizadas...
			request_stream << "GET " << uri.getPathAndQuery() << " HTTP/1.1\r\n";
			

			//request_stream << "Host: " << serverIp << "\r\n";
			//request_stream << "Accept: */*\r\n";
			//request_stream << "Connection: close\r\n";
			//std::string usrAndPwd = "admin:candombe";	//TODO: 
			//std::string credentials = base64_encode(usrAndPwd);
			//request_stream << "Authorization: Basic " << credentials << "\r\n";

			for (HeaderMap::const_iterator it = headers_.begin(); it != headers_.end(); ++it)
			{
				request_stream << (*it).first << ": " << (*it).second << "\r\n";
			}
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
				return "";
			}

			if (status_code != 200)
			{
				std::cout << "Response returned with status code " << status_code << "\n";
				//return 1;
				return ""; 
			}

			// Read the response headers, which are terminated by a blank line.
			boost::asio::read_until(socket, response, "\r\n\r\n");

			// Process the response headers.
			std::string header;
			while (std::getline(response_stream, header) && header != "\r")
			{
				//TODO: 
				//std::cout << header << "\n";
				addResponseHeader(header);
			}
			
			//std::cout << "\n";
			//std::cout << "-------------------- FFFFFFFFFF---------------" << "\n";


			std::stringstream bodyText;

			// Write whatever content we already have to output.
			if (response.size() > 0)
			{
				bodyText <<  &response;
				//std::cout << &response;
				
			}

			// Read until EOF, writing data to output as we go.
			while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
			{
				bodyText << &response;
				//std::cout << &response;
			}
			
			if (error != boost::asio::error::eof)
			{
				throw boost::system::system_error(error);
			}

			//std::cout << bodyText.str();
			return bodyText.str();
		}
		catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << "\n";
		}

		//return 0;
		return "";
	}

protected:

	void parseUri(const std::string& uri, std::string& protocol, std::string& host, std::string& path)
	{
	}

	//TODO: ver si se crea una clase aparte para los Headers.
	virtual void addResponseHeader(const std::string& header)
	{
		//TODO: ver como hacer split cuyo delimitador sea un string completo...
		std::string key;
		std::string value;

		std::vector< std::string > result;
		boost::split( result, header, boost::is_any_of(":") );

		//boost::algorithm::split_regex(
		//boost::first_finder();
		
		this->responseHeaders_[key] = value;

	}

	HeaderMap headers_;
	HeaderMap responseHeaders_;
};



#endif // HTTPCLIENT_HPP