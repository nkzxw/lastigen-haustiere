#include <iostream>

#include "Configuration.hpp"
#include "ConfigManager.hpp"


int main(int argc, char** argv)
{
	ConfigManager<Configuration> cm(argv[0], false);
	//std::cout << cm.getConfigClass().something_important_;
	//std::cout << cm->registrations_.dllPath_;

	//Registration tempReg;
	//tempReg.class_ = "TpLinkManager";
	//tempReg.dllPath_ = "TPLinkManager.dll";
	//cm->registrations_.push_back(tempReg);
	//cm->registrations_.push_back(tempReg);

	cm->mappings_["TpLinkManager"] = "TPLinkManager.dll";
	cm->mappings_["Otro"] = "Otro.dll";

	cm.save();

	return 0;
}





