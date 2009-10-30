//TODO: pasar a proyecto aparte, independiente

#ifndef APMANAGER_HPP_
#define APMANAGER_HPP_

#include <vector>

#include <boost/thread.hpp>

#include "AppSettings.hpp"
#include "AbstractAPController.hpp"
#include "ThreadedClass.hpp"


namespace APManagerState
{
	enum APManagerState {Initializing = 0, APNotConnected = 1, APConnecting = 2, APConnected = 3, Closing = 99};
}


//TODO: ver de usar thread_group de boost::thread

//TODO: le pongo una interface ????
class APManager  : public ThreadedClass<APManager>
{
public:
	friend class ThreadedClass<APManager>; //TODO: ver como hacer para no estar obligado a poner esta declaracion friend. Deberia buscarle la vuelta para que con solo heredar, esto se haga solo, al igual que poner el constructor de la clase protected.

	// Para acceso desde el exterior de la clase
	virtual std::vector<Router> getRouterList() const
	{
		return this->routers_;
	}

	virtual void refreshAllData()
	{
		//TODO: evaluar si todos estos "refresh" abren una conexion TCP cada uno. En tal caso, deber�a mantenerse abierta un tiempo mientras se hacen todos los refresh, recien ahi cerrarse.
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

	//Thread Entry-point
	virtual void doWork()	//TODO: poner un nombre mejor 
	{
		startStateMachine();
	}

	virtual ~APManager()
	{
		std::cout << "virtual ~APManager()" << std::endl;
	}


protected:
	//TODO: ver si conviene inicializar la DLL's en multiples hilos simultaneamente, o en serie, uno despu�s de otro. Probar ambos mecanismos y ver cual es m�s performante
	APManager(const APInformation& apInformation)
		: apInformation_(apInformation), state_(APManagerState::Initializing)
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

    void startStateMachine()
	{
		std::cout << "starting state machine" << std::endl;
		//TODO: ac� deber�amos obtener el status del AP. Entonces, en la maquina, deberiamos agregar un estado llamado GettingInformation

		//TODO: implementar un CONDITIONAL para que la forma de salir no sea esta, as� se puede salir del Wait autom�ticamente.
		while (! stopRequested_)
		{
			switch (state_)
			{
				case APManagerState::Initializing:
					std::cout << apInformation_.name_ << " - Initializing" << std::endl; //TODO: logger
					if ( isConnected() )
					{
						state_ = APManagerState::APConnected;
					}
					else
					{
						state_ = APManagerState::APNotConnected;
					}
					break;
				case APManagerState::APNotConnected:
					std::cout << apInformation_.name_ << " - APNotConnected: finding connections" << std::endl;
					boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
					break;
				case APManagerState::APConnecting:
					std::cout << apInformation_.name_ << " - APConnecting" << std::endl;
					boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
					break;
				case APManagerState::APConnected:
					std::cout << apInformation_.name_ << " - APConnected" << std::endl;

					int millisecondsToWait = ConfigManager<AppSettings>::instance->get<int>("APManagerState.APConnected.MillisecondsToWait");
					boost::this_thread::sleep(boost::posix_time::milliseconds(millisecondsToWait)); //TODO: pasar al archivo de configuraci�n el tiempo de chequeo
					//boost::this_thread::sleep(boost::posix_time::milliseconds(15000)); //TODO: pasar al archivo de configuraci�n el tiempo de chequeo


					//TODO: ver de cambiar por el equivalente a Monitor.Wait(x) porque si el thread est� en sleep no es posible "avisarle" de su destruccion...

					if ( isConnected() )
					{
						state_ = APManagerState::APConnected;
					}
					else
					{
						state_ = APManagerState::APNotConnected;
					}

					break;
				//case APManagerState::Closing:
				//	return;
			}
		}
	}


	bool isConnected()
	{
		return (controller_->getStatus().get() != 0);
	}

	APManagerState::APManagerState state_;

	//shared_ptr
	std::vector<Router> routers_;
	
	//shared_ptr
	Router connectedRouter_; //TODO: debe ser un puntero. Un puntero a un Router del vector de Routers.

	//shared_ptr
	AbstractAPController *controller_;

	APInformation apInformation_;

};

#endif //APMANAGER_HPP_
