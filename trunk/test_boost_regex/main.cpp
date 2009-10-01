
#include <iostream>
#include <string>
#include <boost/regex.hpp>

using namespace std;

void parseUrl( const std::string& url)
{
	//std::string regExStr = "(ftp|http|https):\\/\\/((\\w+\\.)*(\\w*))\\/([\\w\\d]+\\/{0,1})+"; 

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


*/	

	/*

	std::string regExStr = "(ftp|http|https)
		:\\/\\/
		(
			(\\w+\\.)*
			(\\w*)
		)
		\\/
		([\\w\\d]+\\/{0,1})+"; 


	std::string regExStr = "(ftp|http|https)
		:\\/\\/
		(^:+)
		\\/
		([\\w\\d]+\\/{0,1})+"; 

	std::string regExStr = "(ftp|http|https)
		:\\/\\/
		(^:+)
		(:[\\d]{1,5})?
		"; 


*/


	//std::string regExStr = "(ftp|http|https):\\/\\/((\\w+\\.)*(\\w*))\\/([\\w\\d]+\\/{0,1})+"; 
	//std::string regExStr = "(ftp|http|https):\\/\\/(^:+)\\/([\\w\\d]+\\/{0,1})+"; 
	std::string regExStr = "(ftp|http|https):\\/\\/(^:+)(:[\\d]{1,5})?"; 


	boost::regex regEx;
	boost::cmatch matches;

	try
	{
	// Assignment and construction initialize the FSM used for regexp parsing
		regEx = regExStr;
	}
	catch (boost::regex_error& e)
	{
		cout << regExStr << " is not a valid regular expression: \"" << e.what() << "\"" << endl;
		return;
	}


	// if (boost::regex_match(s.begin(), s.end(), regEx))
	//if (boost::regex_match(url, matches, regEx))
	if (boost::regex_match(url.c_str(), matches, regEx))
	{
		// matches[0] contains the original string.  matches[n] contains a sub_match object for each matching subexpression
		for (int i = 1; i < matches.size(); i++)
		{
			// sub_match::first and sub_match::second are iterators that refer to the first and one past the last chars of the matching subexpression
			string match(matches[i].first, matches[i].second);
			cout << "\tmatches[" << i << "] = " << match << endl;
		}
	}
	else
	{
		cout << "The regexp \"" << regExStr << "\" does not match \"" << url << "\"" << endl;
	}

}


int main( ) 
{
  
	//std::string url = "http://www.foo.com/bar";
	std::string url = "http://www.foo.com:8080";
	
	parseUrl(url);

	std::cin.sync();
	std::cin.get();

}

//----------------------------------------------------------------------------------------------------


//int main(int argc, char **argv)
//{
//
//	std::string html = "<ul><li><a href=\"http://www.clarin.com/shared/v9/moviles/\" title=\"Clar&iacute;n M&oacute;viles\" target=\"_blank\">Clar&iacute;n M&oacute;viles</a></li><li><a href=\"http://www.clarin.com/shared/v9/moviles/sms.html\" title=\"Clar&iacute;n SMS\" target=\"_blank\">Clar&iacute;n SMS</a></li><li><a href=\"http://www.clarin.com/shared/v9/moviles/pda.html\" title=\"Versi&oacute;n Palm\" target=\"_blank\">Versi&oacute;n Palm</a></li><li><a href=\"http://toolbar.clarin.com/\" title=\"Toolbar Clar&iacute;n.com\" target=\"_blank\">Toolbar Clar&iacute;n.com</a></li><li><a href=\"http://www.clarin.com/shared/v9/rss.html\" title=\"Noticias RSS\" target=\"_blank\">Noticias RSS</a></li><li><a href=\"http://www.clarin.com/servicios/rss/lector/v8/help.html\" title=\"Lector de noticias RSS\" target=\"_blank\">Lector de noticias RSS</a></li><li><a href=\"http://www.clarin.com/shared/v9/rss.html\" title=\"Noticias gratis en su sitio\" target=\"_blank\">Noticias gratis en su sitio</a></li><!--li><a href=\"http://www.clarin.com/shared/v9/moviles/podcast.html\" title=\"Podcasting\" target=\"_blank\">Podcasting</a></li--><li><a href=\"http://www.clarin.com/shared/v9/ayuda-clarin/index.html\" title=\"Ayuda\" target=\"_blank\">Ayuda</a></li><li><a href=\"http://www.clarin.com/servicios/edicionesanteriores/\" title=\"Archivo\" target=\"_blank\">Archivo</a></li><li><a href=\"http://www.buscador.clarin.com/\" title=\"Buscador\" target=\"_blank\">Buscador</a></li></ul>";
//
//	std::string fulls = html;
//	boost::regex re("<a href=\"([^\"]*)\"");
//	boost::sregex_iterator begin(fulls.begin(), fulls.end(), re), end;
//
//	
//	
//
//
//	std::string sub;
//
//	for (;begin!=end;++begin)
//	{
//		boost::smatch const &what = *begin;
//		//sub = CString(what[0].first,what[0].length());
//		sub = ""; //what[0].first;
//
//		//std::cout << what[0].first ;
//
//		if ( what[0].matched )
//		{
//			what[0].first ;
//			//printf(what[0].first) ;
//
//			//AfxMessageBox(sub);
//			std::cout << sub << std::endl;
//
//		}
//		
//		
//	}
//
//	return 0;
//}

//----------------------------------------------------------------------------------------------------

//#define BOOST_REGEX_MATCH_EXTRA
//
//
//
//#include <boost/regex.hpp>
//#include <iostream>
//
//
//void print_captures(const std::string& regx, const std::string& text)
//{
//   boost::regex e(regx);
//   boost::smatch what;
//   std::cout << "Expression:  \"" << regx << "\"\n";
//   std::cout << "Text:        \"" << text << "\"\n";
//   if(boost::regex_match(text, what, e, boost::match_extra))
//   {
//      unsigned i, j;
//      std::cout << "** Match found **\n   Sub-Expressions:\n";
//      for(i = 0; i < what.size(); ++i)
//         std::cout << "      $" << i << " = \"" << what[i] << "\"\n";
//      std::cout << "   Captures:\n";
//      for(i = 0; i < what.size(); ++i)
//      {
//         std::cout << "      $" << i << " = {";
//         for(j = 0; j < what.captures(i).size(); ++j)
//         {
//            if(j)
//               std::cout << ", ";
//            else
//               std::cout << " ";
//            std::cout << "\"" << what.captures(i)[j] << "\"";
//         }
//         std::cout << " }\n";
//      }
//   }
//   else
//   {
//      std::cout << "** No Match found **\n";
//   }
//}
//
//int main(int , char* [])
//{
//   print_captures("(([[:lower:]]+)|([[:upper:]]+))+", "aBBcccDDDDDeeeeeeee");
//   print_captures("(.*)bar|(.*)bah", "abcbar");
//   print_captures("(.*)bar|(.*)bah", "abcbah");
//   print_captures("^(?:(\\w+)|(?>\\W+))*$", "now is the time for all good men to come to the aid of the party");
//   return 0;
//}



