// http://boost-sandbox.sourceforge.net/libs/xpressive/doc/html/boost_xpressive/user_s_guide/grammars_and_nested_matches.html


#ifndef URI_HPP_
#define URI_HPP_

#include <iostream>
#include <boost/xpressive/xpressive.hpp>

using namespace boost::xpressive;

//TODO: ver en la clase de .Net si permite modificar los atributos una vez que la clase esté construida...
//TODO: ver si es necesario constructor parameterless, en este caso, hay que crear un metodo initialize(...)
//TODO: pasar a StaticRegExp
//TODO: agregar otros protocolos soportados
#define REG_EXP_STR "^(?:(?P<protocol>(?:ftp|http|https))\\:\\/\\/|~/|/)?(?:(?P<username>\\w+):(?P<password>\\w+)@)?(?P<host>[-\\w\\d\\.]+)(?::(?P<port>[\\d]{1,5}))?(?P<path>(?:(?:/(?:[-\\w~!$+|.,=]|%[a-f\\d]{2})+)+|/)+|\\?|#)?(?:\\?(?P<query>(?:(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)(?:&(?:[-\\w~!$+|.,*:]|%[a-f\\d{2}])+=(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*)*))*(?:#(?P<anchor>(?:[-\\w~!$+|.,*:=]|%[a-f\\d]{2})*))?$"

namespace net
{

class Uri
{
public:

	explicit Uri(const std::string& uriStr)
		: valid(false), originalUri(uriStr)//, regularExpression(sregex::compile(REG_EXP_STR))
	{
		smatch what;
		
		if(regex_match(uriStr, what, regularExpression))
		{
			protocol = what["protocol"];
			username = what["username"];
			password = what["password"];
			host = what["host"];
			port = what["port"];
			path = what["path"];
			query = what["query"];
			anchor = what["anchor"];

			//TODO: ver las cosas que faltan y tomar valores por defecto...
			valid = true;
		}
	}

	Uri(const Uri& other)
	{
		originalUri = other.originalUri;
		completeUri = other.completeUri;
		protocol = other.protocol;
		username = other.username;
		password = other.password;
		host = other.host;
		port = other.port;
		path = other.path;
		query = other.query;
		anchor = other.anchor;

		valid = other.valid;
	}


	//TODO: constructor copia, assig op, etc...


	//TODO: poner los atributos protected y generar getters/setters...
	bool valid;
	std::string completeUri;
	std::string originalUri;
	std::string protocol;
	std::string username;
	std::string password;
	std::string host;
	std::string port;
	std::string path;
	std::string query;
	std::string anchor;


	//TODO: esta variable tiene que ser statica, general para todas las instancias de la clase... es en vano compilarla N veces...
	static const sregex regularExpression; // = sregex::compile(regExStr);

};


const sregex Uri::regularExpression = sregex::compile(REG_EXP_STR);

}

#endif // URI_HPP_