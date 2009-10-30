#include <iostream>

#include "AppSettings.hpp"
#include "ConfigManager.hpp"
#include "Router.hpp"

//typedef ConfigManager<AppSettings>::instance CfgMgr;
#define CONFIG ConfigManager<AppSettings>::instance 

int main(int argc, char** argv)
{
	//ConfigManager<AppSettings>::instance->initialize( argv[0], false );
	CONFIG->initialize( argv[0], false );

	//AppSettings *settings = ConfigManager<AppSettings>::instance->getSettings();
	AppSettings *settings = CONFIG->getCustomSettings();

	//ConfigManager<AppSettings>::CommonSettingsType *commonSettings = CONFIG->getCommonSettings();
	//commonSettings ->set("APManagerState.APConnected.MillisecondsToWait", "15000");
	//std::string tempXXX = commonSettings ->get("APManagerState.APConnected.MillisecondsToWait");


	CONFIG->set("APManagerState.APConnected.MillisecondsToWait", 15000);
	int tempXXX = CONFIG->get<int>("APManagerState.APConnected.MillisecondsToWait");

	settings->typeMapping_["TpLinkController"] = "TpLinkController.dll";
	settings->typeMapping_["Otro"] = "Otro.dll";

	//cfg->accessPoints_.push_back(APInformation("tplink", "TpLinkController", "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid", "http://192.168.0.254/userRpm/WlanModeRpm.htm?staSsid=&staType=1&staBssid=&rptBssid=&apMode=4&urptBssid=00-15-6D-53-55-DD&pptBssid=&mptBssid1=&mptBssid2=&mptBssid3=&mptBssid4=&mptBssid5=&mptBssid6=&Save=Save", "admin:candombe"));
	settings->accessPoints_.push_back(APInformation("tplink", "TpLinkController", "http", "192.168.1.254", "admin:candombe"));
	settings->accessPoints_.push_back(APInformation("tplink2", "TpLinkController", "http", "192.168.1.252", "admin:candombe"));


	settings->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	settings->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	settings->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	settings->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));

	//ConfigManager<AppSettings>::instance->save();
	CONFIG->save();

	return 0;
}





