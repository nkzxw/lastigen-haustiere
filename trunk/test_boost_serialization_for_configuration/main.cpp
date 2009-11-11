#include <iostream>

#include <boost/smart_ptr/shared_ptr.hpp>

#include "AppSettings.hpp"
#include "ConfigManager.hpp"
#include "Router.hpp"

//typedef ConfigManager<AppSettings>::instance CfgMgr;
#define CONFIG ConfigManager<AppSettings>::instance 



int main(int argc, char** argv)
{

	//TODO: crear clase BaseApplication, toda aplicacion que quiera contar con caracteristicas de Config, Logger, etc, deberá heredar de ella.
	CONFIG->initialize( argv[0] );

	AppSettings *settingsPointer = CONFIG->getCustomSettings();
	ReferenceConfigAccess<AppSettings> settings = CONFIG->getCustomSettingsLock(); //TODO: el nombre del metodo tiene que cambiar...

	while (true) 
	{
		std::cout << "accessing to the variables via pointer..." << std::endl;
		//int wait = CONFIG->get<int>("APManagerState.APConnected.MillisecondsToWait");
		int wait = 0;
		std::string dll = settings->typeMapping_["Otro"];
		std::string dll2 = settingsPointer->typeMapping_["Otro"];


		//std::cout << "settings: " << settings << std::endl;
		std::cout << "wait: " << wait << std::endl;
		std::cout << "dll: " << dll << std::endl;
		std::cout << "dll2: " << dll2 << std::endl;

		boost::this_thread::sleep(boost::posix_time::milliseconds(12000));
	}





		//boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
		//{
		//std::cout << "trying to access to data..." << std::endl;
		//ReferenceConfigAccess<custom_class> configAccess = c.getAccesor();

		//std::cout << "begin data accessing ... (locked)" << std::endl;
		//outScopePtr = configAccess.operator ->();

		////boost::this_thread::sleep(boost::posix_time::milliseconds(2000));

		//std::cout << "Refresh: " << configAccess->data_ << std::endl;
		//}
		//std::cout << "end data accessing ... (unlocked)" << std::endl;


		//std::cout << "********* WARNING UNSECURED DATA (no-lock): "  << outScopePtr->data_ << std::endl;


		//boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
		//{
		//std::cout << "trying to access to data..." << std::endl;
		//std::cout << "begin data accessing ... (locked)" << std::endl;
		//custom_class tempValue = c.getValue();
		//std::cout << "end data accessing ... (unlocked)" << std::endl;

		//boost::this_thread::sleep(boost::posix_time::milliseconds(2000));

		//std::cout << "Value: " << tempValue.data_ << std::endl;
		//}




	//while (true) // check the internal state of the connection to make sure it's still running
	//{
	//	//boost::this_thread::sleep(boost::posix_time::milliseconds(300));

	//	char ch;
	//	std::cin.get(ch); // blocking wait for standard input
	//	//if (ch == 3) // ctrl-C to end program
	//	if (ch == 'z') // ctrl-C to end program
	//	{
	//		break;
	//	}
	//}


	std::cin.sync();
	std::cin.get();
	return 0;

	//------------------------------------------------------------------------------------------------------------


	////ConfigManager<AppSettings>::instance->initialize( argv[0], false );
	//CONFIG->initialize( argv[0], false, false);

	////AppSettings *settings = ConfigManager<AppSettings>::instance->getSettings();
	//AppSettings *settings = CONFIG->getCustomSettings();
	////ReferenceConfigAccess<AppSettings> settings = CONFIG->getCustomSettingsLock(); //TODO: el nombre del metodo tiene que cambiar...


	////ConfigManager<AppSettings>::CommonSettingsType *commonSettings = CONFIG->getCommonSettings();
	////commonSettings ->set("APManagerState.APConnected.MillisecondsToWait", "15000");
	////std::string tempXXX = commonSettings ->get("APManagerState.APConnected.MillisecondsToWait");


	//CONFIG->set("APManagerState.APConnected.MillisecondsToWait", 15000);
	//int tempXXX = CONFIG->get<int>("APManagerState.APConnected.MillisecondsToWait");

	//settings->typeMapping_["TpLinkController"] = "TpLinkController.dll";
	//settings->typeMapping_["Otro"] = "Otro.dll";

	////cfg->accessPoints_.push_back(APInformation("tplink", "TpLinkController", "http://192.168.0.254/userRpm/popupSiteSurveyRpm.htm?iMAC=urptBssid", "http://192.168.0.254/userRpm/WlanModeRpm.htm?staSsid=&staType=1&staBssid=&rptBssid=&apMode=4&urptBssid=00-15-6D-53-55-DD&pptBssid=&mptBssid1=&mptBssid2=&mptBssid3=&mptBssid4=&mptBssid5=&mptBssid6=&Save=Save", "admin:candombe"));
	//settings->accessPoints_.push_back(APInformation("tplink", "TpLinkController", "http", "192.168.1.254", "admin:candombe"));
	//settings->accessPoints_.push_back(APInformation("tplink2", "TpLinkController", "http", "192.168.1.252", "admin:candombe"));


	//settings->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	//settings->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	//settings->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));
	//settings->knownRouters_.push_back(Router("FF-FF-FF-FF-FF-FF", "xxx", 0, 0, false, "192.168.0.1"));

	////ConfigManager<AppSettings>::instance->save();
	//CONFIG->save();

	//------------------------------------------------------------------------------------------------------------

	return 0;
}





