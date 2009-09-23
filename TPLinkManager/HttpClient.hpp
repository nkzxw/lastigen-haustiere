#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#include <iostream>
#include <istream>
#include <map>
#include <ostream>
#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class HttpClient
{
public:

	typedef std::map<std::string, std::string> HeaderMap;

	virtual void addHeader(const std::string& key, const std::string& value)
	{
		headers_[key] = value;
	}

	virtual void openRead(const std::string& uri)
	{
		std::string protocol;
		std::string host;
		std::string path;

		parseUri(uri, protocol, host, path);

		//TODO: eliminar
		protocol = "http";
		//host = "192.168.0.254";
		host = "www.google.com";
		//path = "/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid";
		path = "/";


		try
		{
			boost::asio::io_service io_service;

			// Get a list of endpoints corresponding to the server name.
			tcp::resolver resolver(io_service);
			tcp::resolver::query query(host, protocol);
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
			request_stream << "GET " << path << " HTTP/1.1\r\n";

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

protected:

	void parseUri(const std::string& uri, std::string& protocol, std::string& host, std::string& path)
	{
	}

	//TODO: usar unorderer_map
	HeaderMap headers_;
	HeaderMap responseHeaders_;
};



#endif // HTTPCLIENT_HPP