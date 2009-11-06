Hola nuevamente!

Ac� vuelvo con otra consulta. Esta vez no es sobre C++ en s�, sino sobre una implementaci�n puntual.
Les cuento un poco que estoy intentando hacer.

Estoy codificando una clase para administrar la configuraci�n de una aplicaci�n en general.
Lo que estoy haciendo es una clase gen�rica de este tipo...

template <typename T>
class ConfigManager;

En esta clase aplico el patr�n Singleton, para poder acceder a la �nica instancia de forma f�cil desde cualquier parte del c�digo.

Al principio del programa ejecuto un m�todo initialize donde le paso argv[0] m�s que nada para saber la ruta del ejecutable.
En esta misma ruta deber�a estar el archivo de configuraci�n que se tiene que llamar igual que el ejecutable, pero con extensi�n .xml .

Algo as�...

int main(int argc, char** argv)
{
	ConfigManager<CustomAppConfig>::instance->initialize( argv[0] );
	//...
}

Una vez hecho esto, desde cualquier parte del c�digo puedo hacer algo como esto...

void x()
{
	ConfigManager<CustomAppConfig>::instance->lo_que_sea();
	//...
}

La clase CustomAppConfig es una clase particular a la aplicaci�n que estoy desarrollando. Es la que contiene la configuraci�n propiamente dicha.
Eleg� usar Boost::Serialization para administrar la configuraci�n. O sea, serializo la configuraci�n en un archivo XML y lo levanto usando Serialization de Boost.
De esta forma, la clase CustomAppConfig tiene una estructura que se carga cuando "de-serializo" del archivo XML.

Hasta aqu�, quiz�s di demasiados detalles innecesarios que no vienen al problema (igualmente si alguno nota alguna falla de dise�o, bienvenido el comentario o sugerencia)

El problema est� en que quiero implementar una forma de "auto-refrescar" la configuraci�n del XML cada cierto tiempo.
Para ello use boost::thread para que cuando se ejecuta el m�todo initialize de ConfigManager, se dispare otro Thread que cada tanto va a chequear si el archivo sufrio cambios o no (quiz�s usando md5sum o similar...) y si el archivo cambio, recargar la configuraci�n.

Ahora... la clase ConfigManager tiene la configuraci�n basada la clase T que es Customizable.
Tiene un m�todo p�blico para obtener la configuraci�n desde afuera de la clase y un m�todo privado que se encarga de refrescar la config. del archivo XML que corre en un thread separado al principal (el thread se crea y ejecuta en el initialize de ConfigManager)
Algo as�...

template <typename T>
class ConfigManager
{
public:


	T* getSettings() 
	{ 
		// LOCK POR SCOPE
		return &settings_;
	}
private:
	void refresh()	//Corre en otro thread
	{
		// LOCK POR SCOPE
		loadConfigurationFromConfigFile(); 
	}

	T settings_;
};

El problema, es que me he dado cuenta tarde de que por m�s que ponga un LOCK en getSettings, este m�todo retorna un puntero.
Por lo tanto, podr�a hacer algo como esto...

//supongamos esta clase de configuraci�n personalizada y una funci�n que use la configuraci�n...

struct CustomAppConfig
{
	std::string s; 
	int i;
};

void codigoQueUsaLaConfiguracion()
{
	CustomAppConfig *cfg = ConfigManager<CustomAppConfig>::instance->getSettings(); //LOCK

	std::cout << cfg->s << std::endl;

	// refresh() comienza a ejecutarse en otro thread...

	std::cout << cfg->i << std::endl;

}

Cuando se ejecuta refresh(), el LOCK no tiene sentido, ya que yo puedo acceder directamente a settings_ por el puntero que ya obtuve.
Entonces cuando se ejecuta "std::cout << cfg->i << std::endl;" se podr�a estar actualizando settings_ lo cual puede traer alguna inconsistencia.

Para solucionar esto se me ocurrieron algunas posibles soluciones...

1. Implementar un LockingProxy (Alexandrescu)
   ConfigManager me quedar�a as�...

template <typename T>
class ConfigManager
{
public:


	LokingProxy<T>* getSettings() 
	{ 
		// LOCK POR SCOPE
		return settings_;
	}
private:
	void refresh()	//Corre en otro thread
	{
		// LOCK POR SCOPE
		loadConfigurationFromConfigFile(); 
	}

	LokingProxy<T>* settings_;
};

En la clase LokingProxy usar�a operator-> para acceder a los miembros de T. En cada llamada a operator-> har�a un LOCK y luego dar�a acceso al miembro de T.

void codigoQueUsaLaConfiguracion()
{
	LokingProxy<CustomAppConfig>* cfg = ConfigManager<CustomAppConfig>::instance->getSettings();
	
	std::cout << cfg->s << std::endl; // LOCK

	// refresh() comienza a ejecutarse en otro thread...

	std::cout << cfg->i << std::endl; // LOCK

}

El problema que encontr� en esta "soluci�n" es que si mi CustomAppConfig retorna punteros o referencias a clases internas, estar�a en el mismo problema...

class CustomAppConfig
{
public:

	std::string* getS()
	{
		return s;
	}
private:
	std::string s; 
	int i;
};

void codigoQueUsaLaConfiguracion()
{
	LokingProxy<CustomAppConfig>* cfg = ConfigManager<CustomAppConfig>::instance->getSettings();

	std::string* s = cfg->getS(); //LOCK
	std::cout << s << std::endl;  //NO LOCK

	// refresh() comienza a ejecutarse en otro thread...

	std::cout << s << std::endl; //NO LOCK

}


2. La segunda soluci�n que se me ocurri� es:
	Usar un atomic swap para intercambiar la configuraci�n actual por la nueva (punteros a la configuraci�n), sin usar MUTEX
	Pero... estoy en la misma...

void codigoQueUsaLaConfiguracion()
{
	CustomAppConfig* cfg = ConfigManager<CustomAppConfig>::instance->getSettings(); 

	std::cout << cfg->s << std::endl; 

	// refresh() comienza a ejecutarse en otro thread...
	// se hace el atomic swap
	// ahora ConfigManager<CustomAppConfig>::instance->getSettings() != cfg !!!!!

	std::cout << cfg->i << std::endl;

	//Entonces... cfg->i es configuraci�n NO ACTUALIZADA...
}


Agradecer�a, comentarios, sugerencias, que Daniel ( Gustavo para los amigos :) ) me gu�e aplicando may�utica, etc.
Quiz�s ya existe una soluci�n similar y estoy re-inventando la rueda...

Si a alguno le interesa el c�digo me avisa, se lo puedo compartir.

Bueno, espero no haberlos aburrido y cansado con este mail tan extenso.

GRACIAS A TODOS!
Saludos.
