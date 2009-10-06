// http://boost-sandbox.sourceforge.net/libs/xpressive/doc/html/boost_xpressive/user_s_guide/grammars_and_nested_matches.html


#ifndef URI_HPP_
#define URI_HPP_

#include <iostream>
#include <boost/xpressive/xpressive.hpp>

using namespace boost::xpressive;

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
		: valid_(false), originalUri_(uriStr)//, regularExpression(sregex::compile(REG_EXP_STR))
	{
		parse(uriStr);
	}

	explicit Uri(const std::string& protocol, const std::string& host, const std::string& pathAndQuery)
		: valid_(false)
	{
		std::string uriStr = protocol + "://" + host;
		
		if (pathAndQuery.size() > 0 && pathAndQuery[0] == '/')
		{
			uriStr += pathAndQuery;
		}
		else
		{
			uriStr += "/" + pathAndQuery;
		}

		parse(uriStr);
	}


	Uri(const Uri& other)
	{
		originalUri_ = other.originalUri_;
		//completeUri_ = other.completeUri_;
		protocol_ = other.protocol_;
		username_ = other.username_;
		password_ = other.password_;
		host_ = other.host_;
		port_ = other.port_;
		path_ = other.path_;
		query_ = other.query_;
		anchor_ = other.anchor_;

		valid_ = other.valid_;
	}


	//TODO: assig op, etc...


	std::string getPathAndQuery() const
	{
		return (this->path_ + "?" + this->query_);
	}


	//TODO: esta variable tiene que ser statica, general para todas las instancias de la clase... es en vano compilarla N veces...
	static const sregex regularExpression; // = sregex::compile(regExStr);

	std::string getCompleteUri() const
	{
		//TODO: ver que partes están definidas y que partes no...
		//TODO: string stream para concatenar, para hacerlo más eficiente.
		return protocol_ + "@" + username_ + ":" + password_ + "://" + host_ + ":" + port_ + "/" + path_ + "?" + query_ + "#" + anchor_;
	}
	std::string getOriginalUri() const
	{
		return originalUri_;
	}

	std::string getProtocol() const
	{
		return protocol_;
	}

	std::string getUsername() const
	{
		return username_;
	}

	std::string getPassword() const
	{
		return password_;
	}

	std::string getHost() const
	{
		return host_;
	}

	std::string getPort() const
	{
		return port_;
	}

	std::string getPath() const
	{
		return path_;
	}

	std::string getQuery() const
	{
		return query_;
	}

	std::string getAnchor() const
	{
		return anchor_;
	}

	bool isValid() const
	{
		return valid_;
	}

protected:

	void parse(const std::string& uriStr)
	{
		smatch what;
		
		if(regex_match(uriStr, what, regularExpression))
		{
			protocol_ = what["protocol"];
			username_ = what["username"];
			password_ = what["password"];
			host_ = what["host"];
			port_ = what["port"];
			path_ = what["path"];
			query_ = what["query"];
			anchor_ = what["anchor"];

			//TODO: ver las cosas que faltan y tomar valores por defecto...
			valid_ = true;
		}
	}





	bool valid_;
	//std::string completeUri_;
	std::string originalUri_;
	std::string protocol_;
	std::string username_;
	std::string password_;
	std::string host_;
	std::string port_;
	std::string path_;
	
	//TODO: el query puede ser otra clase que sea una lista de nombre/valor
	std::string query_;

	std::string anchor_;
};


const sregex Uri::regularExpression = sregex::compile(REG_EXP_STR);

}

#endif // URI_HPP_