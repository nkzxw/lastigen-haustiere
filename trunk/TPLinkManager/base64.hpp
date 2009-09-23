#ifndef BASE64_HPP	
#define BASE64_HPP

#include <string>
//#include <iostream>

#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
 
using namespace std;
using namespace boost::archive::iterators;

//inline String base64_encode(InputContainerType const & cont) 

template<typename InputContainerType> 
inline std::string base64_encode(const InputContainerType const & cont) 
{ 
	using namespace boost::archive::iterators; 
	//typedef base64_from_binary< transform_width< typename InputContainerType::const_pointer, 6, 8, 	boost::uint8_t	> > base64_t; 
	typedef base64_from_binary< transform_width< typename InputContainerType::const_pointer, 6, 8 > > base64_t; 
	//return String(base64_t(&cont[0]), base64_t(&cont[0] + cont.size())); 


	std::string str = std::string(base64_t(&cont[0]), base64_t(&cont[0] + cont.size()));
	//str.append(str.size() % 4, CharT('=')); 
	str.append(str.size() % 4, '='); 

	return str; 
} 

template<typename Container, typename CharT> 
inline Container base64_decode(std::basic_string<CharT> str) 
{ 
	//str.append(str.size() % 4, CharT('=')); 
	//typedef boost::archive::iterators::transform_width< boost::archive::iterators::binary_from_base64<CharT *>, 8, 6, boost::uint8_t > binary_t; 
	typedef boost::archive::iterators::transform_width< boost::archive::iterators::binary_from_base64<CharT *>, 8, 6 > binary_t; 

	typename std::basic_string<CharT>::size_type pos = str.find_last_not_of(CharT('=')); 
	// calculate how many characters we need to process 
	pos = (pos == str.size() -1 ? str.size() : pos ); 
	return Container(binary_t(&str[0]), binary_t(&str[0] + pos)); 
} 



//int main()
//{
//
//	string str("admin:1234");
//	cout << str << endl;
//		
//	std::string enc = base64_encode(str);
//
//	cout << enc << endl;
//
//	str = base64_decode<std::string>(enc);
//
//	cout << str << endl;
//
//
//
//
//	//string str("Hello, world!");
//	////string str("admin:1234");
//	//cout << str << endl;
//	////string enc(base64_t(str.begin()), base64_t(str.end()));
//
//	//std::string enc;
//
//
//	//std::copy(
//	//	base64_t(BOOST_MAKE_PFTO_WRAPPER(str.begin())),
//	//	base64_t(BOOST_MAKE_PFTO_WRAPPER(str.end())),
//	//	std::back_inserter(enc)
//	//);
//
//
//
//	//cout << enc << endl;
//	//string dec(binary_t(enc.begin()), binary_t(enc.end()));
//	//cout << dec << endl;
//	return 0;
//}

#endif // BASE64_HPP
