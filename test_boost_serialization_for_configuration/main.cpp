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
	cfg->accessPoints_.push_back(APInformation("tplink", "TpLinkManager", "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid", "http://192.168.0.254/QUEMAS", "admin:candombe"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	cfg->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));


	//Router(const std::string& bssid, const std::string& ssid, int signal, int channel, bool security, std::string knownIpAddress = "")


	cm.save();

	return 0;
}





