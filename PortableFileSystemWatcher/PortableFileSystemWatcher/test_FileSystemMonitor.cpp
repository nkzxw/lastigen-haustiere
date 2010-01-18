#include <iostream>
#include <string>

#include "FileSystemMonitor.hpp"


struct object {};

static void OnChanged(FileSystemEventArgs e) // object source, 
{
	std::cout << "Changed: " << e.Name << std::endl;

	// Specify what is done when a file is changed, created, or deleted.
	//Console.WriteLine("File: " + e.FullPath + " " + e.ChangeType);
}

static void OnCreated(FileSystemEventArgs e) // object source, 
{
	std::cout << "Created: " << e.Name << std::endl;

	// Specify what is done when a file is changed, created, or deleted.
	//Console.WriteLine("File: " + e.FullPath + " " + e.ChangeType);
}

static void OnDeleted(FileSystemEventArgs e) // object source, 
{
	std::cout << "Deleted: " << e.Name << std::endl;

	// Specify what is done when a file is changed, created, or deleted.
	//Console.WriteLine("File: " + e.FullPath + " " + e.ChangeType);
}

static void OnRenamed(RenamedEventArgs e) // object source, 
{
	std::cout << "Renamed: " << e.Name << std::endl;
	// Specify what is done when a file is renamed.
	//Console.WriteLine("File: {0} renamed to {1}", e.OldFullPath, e.FullPath);
}

int main(int argc, char** argv)
{
	//std::string path = "D:\\temp1";
	std::string path = "C:\\temp1";
    
	//TODO: implementar multiples paths????? O conviene que cada objeto FileSystemMonitor administre un path????

	FileSystemMonitor* watcher;


	try
	{
		//FileSystemMonitor watcher;
		//FileSystemMonitor watcher(path);
		watcher = new FileSystemMonitor(path);

		//watcher.setPath( path );
		watcher->setNotifyFilters( NotifyFilters::LastAccess | NotifyFilters::LastWrite | NotifyFilters::FileName | NotifyFilters::DirectoryName );
		watcher->setFilter("*.txt"); //TODO: implementar este filtro
		watcher->setChangedEventHandler(OnChanged);
		watcher->setCreatedEventHandler(OnCreated);
		watcher->setDeletedEventHandler(OnDeleted);
		watcher->setRenamedEventHandler(OnRenamed);

		//watcher->setEnableRaisingEvents(true); //TODO: cambiar, no está bueno este diseño. Crear un método Start.
		watcher->startMonitoring();
		watcher->startMonitoring();	//TODO: esto crearia otro Thread... si lo implementamos como un Setter "EnableRaisingEvents" podemos manejarlo de otra manera...

	}
	catch (std::runtime_error re)
	{
		std::cout << re.what() << std::endl;
	}

	std::cin.get();

	delete watcher;

	return 0;
}
