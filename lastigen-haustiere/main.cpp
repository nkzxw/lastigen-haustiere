#include <iostream>
#include <string>

#include <boost/regex.hpp>

// libboost_regex-vc90-mt-gd-1_40.lib
// libboost_regex-vc90-mt-gd-1_39.lib

// http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid


bool validate_card_format(const std::string& s)
{
   static const boost::regex e("(\\d{4}[- ]){3}\\d{4}");
   return regex_match(s, e);
}


int main(int argc, char **argv)
{
	char tempChar[20];

	std::cin.getline(tempChar, 20);
	std::string creditCardNumber(tempChar);
	bool isValid = validate_card_format(creditCardNumber);

	std::cout << creditCardNumber << " ... is valid: " << isValid << std::endl;

	std::cin.sync();
	std::cin.get();
	return 0;
}
