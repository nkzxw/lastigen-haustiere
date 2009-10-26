//TODO: pasar a proyecto aparte, independiente

#ifndef APMANAGER_HPP_
#define APMANAGER_HPP_

#include <vector>

#include "AppSettings.hpp"
#include "AbstractAPController.hpp"


//TODO: le pongo una interface ????
class APManager
{
public:

	//TODO: ver si conviene inicializar la DLL's en multiples hilos simultaneamente, o en serie, uno después de otro. Probar ambos mecanismos y ver cual es más performante
	APManager(const APInformation& apInformation)
		//: connectedRouter_(0)
	{
		//shared_ptr
		AppSettings *settings = ConfigManager<AppSettings>::instance->getSettings();

		//TODO: poner un metodo en la clase AppSettings para obtener los mappings...
		//TODO: ver que clase puede encargarse de instanciar totalmente los APControllers...
		//TODO: implementar un cache de DLL's ya abiertas para no reabrir la misma DLL muchas veces
		//std::string sharedLibrary = settings.typeMapping_[it->accessPointController_];
		std::string sharedLibrary = settings->typeMapping_.at(apInformation.accessPointController_);

		//shared_ptr
		controller_ = ReflectionManager::CreateInstance<AbstractAPController>(sharedLibrary, apInformation.accessPointController_);
		controller_->initialize(apInformation);
	}


	// Para acceso desde el exterior de la clase
	virtual std::vector<Router> getRouterList() const
	{
		return this->routers_;
	}

	virtual void refreshAllData()
	{
		//TODO: evaluar si todos estos "refresh" abren una conexion TCP cada uno. En tal caso, debería mantenerse abierta un tiempo mientras se hacen todos los refresh, recien ahi cerrarse.
		refreshRouterList();
		refreshConnectedRouter();
	}

	virtual void refreshRouterList()
	{
		this->routers_ = controller_->getRouterList();
	}

	virtual void refreshConnectedRouter()
	{
		this->connectedRouter_ = controller_->getConnectedRouter();
	}



	virtual Router getConnectedRouter() const
	{
		return this->connectedRouter_;
	}




	//TODO: start de la maquina de estados. Debe correr en un thread separado. Quien se encarga de levantar el thread. El SNM o el APManager mismo (desde el constructor)?
	virtual void startStateMachine()
	{
	}

	virtual ~APManager()
	{
		std::cout << "virtual ~APManager()" << std::endl;
	}


protected:
	//shared_ptr
	std::vector<Router> routers_;
	
	//shared_ptr
	Router connectedRouter_; //TODO: debe ser un puntero. Un puntero a un Router del vector de Routers.

	//shared_ptr
	AbstractAPController *controller_;

};

#endif //APMANAGER_HPP_
