#include <iostream>

#include "AppSettings.hpp"
#include "ConfigManager.hpp"
#include "Router.hpp"


int main(int argc, char** argv)
{
	ConfigManager<AppSettings> cm(argv[0], false);
	AppSettings *cfg = cm.getSettings();

	cfg->typeMapping_["TpLinkManager"] = "TPLinkManager.dll";
	cfg->typeMapping_["Otro"] = "Otro.dll";

	//cfg->accessPoints_.push_back(APInformation("tplink", "TpLinkManager", "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid", "http://192.168.0.254/userRpm/WlanModeRpm.htm?staSsid=&staType=1&staBssid=&rptBssid=&apMode=4&urptBssid=00-15-6D-53-55-DD&pptBssid=&mptBssid1=&mptBssid2=&mptBssid3=&mptBssid4=&mptBssid5=&mptBssid6=&Save=Save", "admin:candombe"));
	cfg->accessPoints_.push_back(APInformation("tplink", "TpLinkManager", "http", "192.168.0.254", "admin:candombe"));


	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));



	/*
	/userRpm/WlanModeRpm.htm?apBroadcast=2&staSsid=&staType=1&staBssid=00-1B-9E-CE-4B-BC&rptBssid=&apMode=4&urptBssid=00-02-CF-8F-76-C2&pptBssid=&mptBssid1=&mptBssid2=&mptBssid3=&mptBssid4=&mptBssid5=&mptBssid6=&Save=Save
	/userRpm/WlanModeRpm.htm?apMode=4&urptBssid=00-02-CF-8F-76-C2&Save=Save


	http://192.168.0.254/userRpm/WlanModeRpm.htm?apMode=4&urptBssid=00-0E-2E-85-25-A5&Save=Save
	*/
	
		
	//Router(const std::string& bssid, const std::string& ssid, int signal, int channel, bool security, std::string knownIpAddress = "")


	cm.save();

	return 0;
}





