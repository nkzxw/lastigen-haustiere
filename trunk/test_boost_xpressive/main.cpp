// http://boost-sandbox.sourceforge.net/libs/xpressive/doc/html/boost_xpressive/user_s_guide/grammars_and_nested_matches.html

#include <iostream>
#include <boost/xpressive/xpressive.hpp>

#include "Uri.hpp"

using namespace boost::xpressive;



int main()
{

	//--------------------------------------------------------------------------------------------------------

	net::Uri uri1("http://usuario:password@www.google.com:8080/hola/como/te/va/todo/bien.aspx?name=value&name=value&name=value#fer");
	//uri uri("pepe");

	net::Uri uri2 = uri1;
	
	if (uri1.isValid())
	{
		std::cout << "completeUri: " << uri1.getCompleteUri() << std::endl;
		std::cout << "originalUri: " << uri1.getOriginalUri() << std::endl;
		std::cout << "protocol: " << uri1.getProtocol() << std::endl;
		std::cout << "username: " << uri1.getUsername() << std::endl;
		std::cout << "password: " << uri1.getPassword() << std::endl;
		std::cout << "host: " << uri1.getHost() << std::endl;
		std::cout << "port: " << uri1.getPort() << std::endl;
		std::cout << "path: " << uri1.getPath() << std::endl;
		std::cout << "query: " << uri1.getQuery() << std::endl;
		std::cout << "anchor: " << uri1.getAnchor() << std::endl;
	}


	if ( uri2.isValid() )
	{
		std::cout << "completeUri: " << uri2.getCompleteUri() << std::endl;
		std::cout << "originalUri: " << uri2.getOriginalUri() << std::endl;
		std::cout << "protocol: " << uri2.getProtocol() << std::endl;
		std::cout << "username: " << uri2.getUsername() << std::endl;
		std::cout << "password: " << uri2.getPassword() << std::endl;
		std::cout << "host: " << uri2.getHost() << std::endl;
		std::cout << "port: " << uri2.getPort() << std::endl;
		std::cout << "path: " << uri2.getPath() << std::endl;
		std::cout << "query: " << uri2.getQuery() << std::endl;
		std::cout << "anchor: " << uri2.getAnchor() << std::endl;
	}

	//--------------------------------------------------------------------------------------------------------


	///*
	////std::string regExStr = "^(?#Protocol)(?:(?:ht|f)tp(?:s?)\\:\\/\\/|~/|/)?(?#Username:Password)(?:\\w+:\\w+@)?(?#Subdomains)(?:(?:[-\\w]+\\.)+(?#TopLevel Domains)(?:com|org|net|gov|mil|biz|info|mobi|name|aero|jobs|museum|travel|[a-z]{2}))(?#Port)(?::[\\d]{1,5})?(?#Directories)(?:(?:(?:/(?:[-\\w~!$+|.,=]|%[a-f\\d]{2})+)+|/)+|\\?|#)?(?#Query)(?:(?:\\?(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)(?:&(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)*)*(?#Anchor)(?:#(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)?$ "; 

	//std::string regExStr = "^(?#Protocol)(?:(?:ht|f)tp(?:s?)\\:\\/\\/|~/|/)?
	//	                     (?#Username:Password)(?:\\w+:\\w+@)?
	//						 (?#Subdomains)(?:(?:[-\\w]+\\.)+
	//							(?#TopLevel Domains)(?:com|org|net|gov|mil|biz|info|mobi|name|aero|jobs|museum|travel|[a-z]{2})
	//						  )
	//						  (?#Port)(?::[\\d]{1,5})?
	//						  (?#Directories)(?:(?:(?:/(?:[-\\w~!$+|.,=]|%[a-f\\d]{2})+)+|/)+|\\?|#)?
	//						  (?#Query)(?:(?:\\?(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)(?:&(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)*)*
	//						  (?#Anchor)(?:#(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)?$ "; 

	//std::string regExStr = "^
	//						 (?P<Protocol>(?:ht|f)tp(?:s?)\\:\\/\\/|~/|/)?
	//	                     (?P<Username:Password>\\w+:\\w+@)?
	//						 (?P<Subdomains>(?:[-\\w]+\\.)+
	//							(?P<TopLevel Domains>com|org|net|gov|mil|biz|info|mobi|name|aero|jobs|museum|travel|[a-z]{2})
	//						  )
	//						  (?P<Port>:[\\d]{1,5})?
	//						  (?P<Directories>(?:(?:/(?:[-\\w~!$+|.,=]|%[a-f\\d]{2})+)+|/)+|\\?|#)?
	//						  (?P<Query>(?:\\?(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)(?:&(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)*)*
	//						  (?P<Anchor>#(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)?
	//						  $ "; 
	//*/


	//std::string regExStr = "^(?:(?P<protocol>(?:ftp|http|https))\\:\\/\\/|~/|/)?(?:(?P<username>\\w+):(?P<password>\\w+)@)?(?P<host>[-\\w\\d\\.]+)(?::(?P<port>[\\d]{1,5}))?(?P<path>(?:(?:/(?:[-\\w~!$+|.,=]|%[a-f\\d]{2})+)+|/)+|\\?|#)?(?:\\?(?P<query>(?:(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)(?:&(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)*))*(?:#(?P<anchor>(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*))?$";

	//std::string str( "http://usuario:password@www.google.com:8080/hola/como/te/va/todo/bien.aspx?name=value&name=value&name=value#fer" );

	//smatch what;
	//sregex rx = sregex::compile(regExStr);
	//
	//if(regex_match(str, what, rx))
	//{
	//	std::cout << "protocol: " << what["protocol"] << std::endl;
	//	std::cout << "username: " << what["username"] << std::endl;
	//	std::cout << "password: " << what["password"] << std::endl;
	//	std::cout << "host: " << what["host"] << std::endl;
	//	std::cout << "port: " << what["port"] << std::endl;
	//	std::cout << "path: " << what["path"] << std::endl;
	//	std::cout << "query: " << what["query"] << std::endl;
	//	std::cout << "anchor: " << what["anchor"] << std::endl;

	//	std::cout << "other: " << what["other"] << std::endl;

	//	std::cout << what[0] << std::endl;
	//	std::cout << what[1] << std::endl;
	//	std::cout << what[2] << std::endl;
	//	std::cout << what[3] << std::endl;
	//	//std::cout << what["world"] << std::endl;
	//	//std::cout << what["sign"] << std::endl;
	//}

	//--------------------------------------------------------------------------------------------------------

	//std::string str( "hello world!" );

	//smatch what;
	////sregex rx = sregex::compile("(?P<name>\\w+)");
	//sregex rx = sregex::compile("(?P<hello>\\w+) (?P<world>\\w+)(?P<sign>!)");
	//
	//if(regex_match(str, what, rx))
	//{
	//	std::cout << what["hello"] << std::endl;
	//	std::cout << what["world"] << std::endl;
	//	std::cout << what["sign"] << std::endl;
	//}

	//--------------------------------------------------------------------------------------------------------


 //   std::string hello( "hello world!" );

 //   sregex rex = sregex::compile( "(\\w+) (\\w+)!" );
	//
	////sregex rex = (s1= +_w) >> ' ' >> (s2= +_w) >> '!';

 //   smatch what;

 //   if( regex_match( hello, what, rex ) )
 //   {
 //       std::cout << what[0] << '\n'; // whole match
 //       std::cout << what[1] << '\n'; // first capture
 //       std::cout << what[2] << '\n'; // second capture
 //   }


	//--------------------------------------------------------------------------------------------------------


	////http://lists.boost.org/boost-users/2006/06/20476.php
	//std::string str = "08/05/1979";

 //    // define some custom mark_tags with meaningful names
 //    mark_tag day(1), month(2), year(3), delim(4);

 //    // this regex finds a date, perl equivalent would be:
 //    // (\d{1,2})([/-])(\d{1,2})\2((?:\d\d){1,2}))
 //    sregex date = (month= repeat<1,2>(_d)) // find the month
	//		>> (delim= (set= '/','-')) // followed by a delimiter
	//		>> (day= repeat<1,2>(_d)) // and a day
	//		>> delim // and the delimiter again
	//		>> (year= repeat<1,2>(_d >> _d)); // and the year.

	// 
	// smatch what;

 //    if( regex_search( str, what, date ) )
 //    {
 //        std::cout << what[0] << '\n'; // whole match
 //        std::cout << what[day] << '\n'; // the day
 //        std::cout << what[month] << '\n'; // the month
 //        std::cout << what[year] << '\n'; // the year
 //        std::cout << what[delim] << '\n'; // the delimiter
 //    } 


	//--------------------------------------------------------------------------------------------------------


	std::cin.sync();
	std::cin.get();
    return 0;
}
