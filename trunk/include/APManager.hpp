//TODO: pasar a proyecto aparte, independiente

#ifndef APMANAGER_HPP_
#define APMANAGER_HPP_

#include <vector>

#include "AppSettings.hpp"
#include "AbstractAPController.hpp"



class APManager
{
public:

	//APManager(const APInformation& information)
	//	: routerListObtained_(false), information_(information)
	//{
	//}

	APManager(const APInformation& apInformation)
	{
		////TODO: poner un metodo en la clase AppSettings para obtener los mappings...
		////TODO: ver que clase puede encargarse de instanciar totalmente los APControllers...
		////TODO: implementar un cache de DLL's ya abiertas para no reabrir la misma DLL muchas veces
		////std::string sharedLibrary = settings.typeMapping_[it->accessPointController_];
		//std::string sharedLibrary = settings.typeMapping_.at(apInformation.accessPointController_);

		////shared_ptr
		//controller_ = ReflectionManager::CreateInstance<AbstractAPController>(sharedLibrary, apInformation.accessPointController_);
		//controller_->initialize(apInformation);
	}

	virtual void init()
	{
	}

	virtual ~APManager()
	{
		std::cout << "virtual ~APManager()" << std::endl;
	}


protected:

	//shared_ptr
	AbstractAPController *controller_;

};

#endif //APMANAGER_HPP_
