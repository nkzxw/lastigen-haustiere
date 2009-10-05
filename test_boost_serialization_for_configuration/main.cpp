#include <iostream>

#include "AppSettings.hpp"
#include "ConfigManager.hpp"


int main(int argc, char** argv)
{
	ConfigManager<AppSettings> cm(argv[0], false);
	AppSettings *cfg = cm.getSettings();

	cfg->typeMapping_["TpLinkManager"] = "TPLinkManager.dll";
	cfg->typeMapping_["Otro"] = "Otro.dll";
	cfg->accessPoints_.push_back(AccessPointInformation("tplink", "TpLinkManager", "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid", "http://192.168.0.254/QUEMAS", "admin:candombe"));

	cm.save();

	return 0;
}





