// http://boost-sandbox.sourceforge.net/libs/xpressive/doc/html/boost_xpressive/user_s_guide/grammars_and_nested_matches.html

#include <iostream>
#include <boost/xpressive/xpressive.hpp>

using namespace boost::xpressive;

int main()
{

	//--------------------------------------------------------------------------------------------------------


	/*
	//std::string regExStr = "^(?#Protocol)(?:(?:ht|f)tp(?:s?)\\:\\/\\/|~/|/)?(?#Username:Password)(?:\\w+:\\w+@)?(?#Subdomains)(?:(?:[-\\w]+\\.)+(?#TopLevel Domains)(?:com|org|net|gov|mil|biz|info|mobi|name|aero|jobs|museum|travel|[a-z]{2}))(?#Port)(?::[\\d]{1,5})?(?#Directories)(?:(?:(?:/(?:[-\\w~!$+|.,=]|%[a-f\\d]{2})+)+|/)+|\\?|#)?(?#Query)(?:(?:\\?(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)(?:&(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)*)*(?#Anchor)(?:#(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)?$ "; 

	std::string regExStr = "^(?#Protocol)(?:(?:ht|f)tp(?:s?)\\:\\/\\/|~/|/)?
		                     (?#Username:Password)(?:\\w+:\\w+@)?
							 (?#Subdomains)(?:(?:[-\\w]+\\.)+
								(?#TopLevel Domains)(?:com|org|net|gov|mil|biz|info|mobi|name|aero|jobs|museum|travel|[a-z]{2})
							  )
							  (?#Port)(?::[\\d]{1,5})?
							  (?#Directories)(?:(?:(?:/(?:[-\\w~!$+|.,=]|%[a-f\\d]{2})+)+|/)+|\\?|#)?
							  (?#Query)(?:(?:\\?(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)(?:&(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)*)*
							  (?#Anchor)(?:#(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)?$ "; 

	std::string regExStr = "^
							 (?P<Protocol>(?:ht|f)tp(?:s?)\\:\\/\\/|~/|/)?
		                     (?P<Username:Password>\\w+:\\w+@)?
							 (?P<Subdomains>(?:[-\\w]+\\.)+
								(?P<TopLevel Domains>com|org|net|gov|mil|biz|info|mobi|name|aero|jobs|museum|travel|[a-z]{2})
							  )
							  (?P<Port>:[\\d]{1,5})?
							  (?P<Directories>(?:(?:/(?:[-\\w~!$+|.,=]|%[a-f\\d]{2})+)+|/)+|\\?|#)?
							  (?P<Query>(?:\\?(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)(?:&(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)*)*
							  (?P<Anchor>#(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)?$ "; 
	*/


	std::string regExStr = "^(?:(?P<protocol>(?:ftp|http|https))\\:\\/\\/|~/|/)?(?:(?P<username>\\w+):(?P<password>\\w+)@)?(?P<host>[-\\w\\d\\.]+)(?::(?P<port>[\\d]{1,5}))?(?P<path>(?:(?:/(?:[-\\w~!$+|.,=]|%[a-f\\d]{2})+)+|/)+|\\?|#)?(?P<query>(?:\\?(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)(?:&(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)*)*(?P<other>.*)?$";

	std::string str( "http://usuario:password@www.google.com:8080/hola/como/te/va/todo/bien.aspx?name=value" );

	smatch what;
	sregex rx = sregex::compile(regExStr);
	
	if(regex_match(str, what, rx))
	{
		std::cout << "protocol: " << what["protocol"] << std::endl;
		std::cout << "username: " << what["username"] << std::endl;
		std::cout << "password: " << what["password"] << std::endl;
		std::cout << "host: " << what["host"] << std::endl;
		std::cout << "port: " << what["port"] << std::endl;
		std::cout << "path: " << what["path"] << std::endl;
		std::cout << "query: " << what["query"] << std::endl;

		std::cout << "other: " << what["other"] << std::endl;

		std::cout << what[0] << std::endl;
		std::cout << what[1] << std::endl;
		std::cout << what[2] << std::endl;
		std::cout << what[3] << std::endl;
		//std::cout << what["world"] << std::endl;
		//std::cout << what["sign"] << std::endl;
	}

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
