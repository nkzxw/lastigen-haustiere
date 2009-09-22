#include <iostream>

#include "Configuration.hpp"
#include "ConfigManager.hpp"


int main(int argc, char** argv)
{
	ConfigManager<Configuration> cm(argv[0], false);
	//std::cout << cm.getConfigClass().something_important_;
	std::cout << cm->registration_.dllPath_;

	cm->registration_.interface_ = "AbstractRouterManager";
	cm->registration_.dllPath_ = "TPLinkManager.dll";
	cm.save();

	return 0;
}





