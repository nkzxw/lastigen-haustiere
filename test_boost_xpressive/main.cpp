// http://boost-sandbox.sourceforge.net/libs/xpressive/doc/html/boost_xpressive/user_s_guide/grammars_and_nested_matches.html

#include <iostream>
#include <boost/xpressive/xpressive.hpp>

using namespace boost::xpressive;

int main()
{

	//--------------------------------------------------------------------------------------------------------


    std::string hello( "hello world!" );

    sregex rex = sregex::compile( "(\\w+) (\\w+)!" );
	
	//sregex rex = (s1= +_w) >> ' ' >> (s2= +_w) >> '!';

    smatch what;

    if( regex_match( hello, what, rex ) )
    {
        std::cout << what[0] << '\n'; // whole match
        std::cout << what[1] << '\n'; // first capture
        std::cout << what[2] << '\n'; // second capture
    }


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
