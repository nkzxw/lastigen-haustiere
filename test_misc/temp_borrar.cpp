Hola nuevamente!

Acá vuelvo con otra consulta. Esta vez no es sobre C++ en sí, sino sobre una implementación puntual.
Les cuento un poco que estoy intentando hacer.

Estoy codificando una clase para administrar la configuración de una aplicación en general.
Lo que estoy haciendo es una clase genérica de este tipo...

template <typename T>
class ConfigManager;

En esta clase aplico el patrón Singleton, para poder acceder a la única instancia de forma fácil desde cualquier parte del código.

Al principio del programa ejecuto un método initialize donde le paso argv[0] más que nada para saber la ruta del ejecutable.
En esta misma ruta debería estar el archivo de configuración que se tiene que llamar igual que el ejecutable, pero con extensión .xml .

Algo así...

int main(int argc, char** argv)
{
	ConfigManager<CustomAppConfig>::instance->initialize( argv[0] );
	//...
}

Una vez hecho esto, desde cualquier parte del código puedo hacer algo como esto...

void x()
{
	ConfigManager<CustomAppConfig>::instance->lo_que_sea();
	//...
}

La clase CustomAppConfig es una clase particular a la aplicación que estoy desarrollando. Es la que contiene la configuración propiamente dicha.
Elegí usar Boost::Serialization para administrar la configuración. O sea, serializo la configuración en un archivo XML y lo levanto usando Serialization de Boost.
De esta forma, la clase CustomAppConfig tiene una estructura que se carga cuando "de-serializo" del archivo XML.

Hasta aquí, quizás di demasiados detalles innecesarios que no vienen al problema (igualmente si alguno nota alguna falla de diseño, bienvenido el comentario o sugerencia)

El problema está en que quiero implementar una forma de "auto-refrescar" la configuración del XML cada cierto tiempo.
Para ello use boost::thread para que cuando se ejecuta el método initialize de ConfigManager, se dispare otro Thread que cada tanto va a chequear si el archivo sufrio cambios o no (quizás usando md5sum o similar...) y si el archivo cambio, recargar la configuración.

Ahora... la clase ConfigManager tiene la configuración basada la clase T que es Customizable.
Tiene un método público para obtener la configuración desde afuera de la clase y un método privado que se encarga de refrescar la config. del archivo XML que corre en un thread separado al principal (el thread se crea y ejecuta en el initialize de ConfigManager)
Algo así...

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

El problema, es que me he dado cuenta tarde de que por más que ponga un LOCK en getSettings, este método retorna un puntero.
Por lo tanto, podría hacer algo como esto...

//supongamos esta clase de configuración personalizada y una función que use la configuración...

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
Entonces cuando se ejecuta "std::cout << cfg->i << std::endl;" se podría estar actualizando settings_ lo cual puede traer alguna inconsistencia.

Para solucionar esto se me ocurrieron algunas posibles soluciones...

1. Implementar un LockingProxy (Alexandrescu)
   ConfigManager me quedaría así...

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

En la clase LokingProxy usaría operator-> para acceder a los miembros de T. En cada llamada a operator-> haría un LOCK y luego daría acceso al miembro de T.

void codigoQueUsaLaConfiguracion()
{
	LokingProxy<CustomAppConfig>* cfg = ConfigManager<CustomAppConfig>::instance->getSettings();
	
	std::cout << cfg->s << std::endl; // LOCK

	// refresh() comienza a ejecutarse en otro thread...

	std::cout << cfg->i << std::endl; // LOCK

}

El problema que encontré en esta "solución" es que si mi CustomAppConfig retorna punteros o referencias a clases internas, estaría en el mismo problema...

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


2. La segunda solución que se me ocurrió es:
	Usar un atomic swap para intercambiar la configuración actual por la nueva (punteros a la configuración), sin usar MUTEX
	Pero... estoy en la misma...

void codigoQueUsaLaConfiguracion()
{
	CustomAppConfig* cfg = ConfigManager<CustomAppConfig>::instance->getSettings(); 

	std::cout << cfg->s << std::endl; 

	// refresh() comienza a ejecutarse en otro thread...
	// se hace el atomic swap
	// ahora ConfigManager<CustomAppConfig>::instance->getSettings() != cfg !!!!!

	std::cout << cfg->i << std::endl;

	//Entonces... cfg->i es configuración NO ACTUALIZADA...
}


Agradecería, comentarios, sugerencias, que Daniel ( Gustavo para los amigos :) ) me guíe aplicando mayéutica, etc.
Quizás ya existe una solución similar y estoy re-inventando la rueda...

Si a alguno le interesa el código me avisa, se lo puedo compartir.

Bueno, espero no haberlos aburrido y cansado con este mail tan extenso.

GRACIAS A TODOS!
Saludos.
