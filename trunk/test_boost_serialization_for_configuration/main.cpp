#include <iostream>

#include "AppSettings.hpp"
#include "ConfigManager.hpp"
#include "Router.hpp"


int main(int argc, char** argv)
{
	ConfigManager<AppSettings> cm(argv[0], false);
	AppSettings *cfg = cm.getSettings();

	cfg->typeMapping_["TpLinkController"] = "TpLinkController.dll";
	cfg->typeMapping_["Otro"] = "Otro.dll";

	//cfg->accessPoints_.push_back(APInformation("tplink", "TpLinkController", "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid", "http://192.168.0.254/userRpm/WlanModeRpm.htm?staSsid=&staType=1&staBssid=&rptBssid=&apMode=4&urptBssid=00-15-6D-53-55-DD&pptBssid=&mptBssid1=&mptBssid2=&mptBssid3=&mptBssid4=&mptBssid5=&mptBssid6=&Save=Save", "admin:candombe"));
	cfg->accessPoints_.push_back(APInformation("tplink", "TpLinkController", "http", "192.168.0.254", "admin:candombe"));
	cfg->accessPoints_.push_back(APInformation("tplink-otro", "TpLinkController", "http", "192.168.1.254", "pepe:pepe"));


	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));

	cm.save();

	return 0;
}





