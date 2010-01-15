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
	std::string path = "C:\\temp1";
    

    FileSystemMonitor watcher; // = new FileSystemMonitor;
    watcher.setPath( path );
	watcher.setNotifyFilters( NotifyFilters::LastAccess | NotifyFilters::LastWrite | NotifyFilters::FileName | NotifyFilters::DirectoryName );
	watcher.setFilter("*.txt");

	watcher.Changed = OnChanged;
	watcher.Created = OnCreated;
	watcher.Deleted = OnDeleted;
	watcher.Renamed = OnRenamed;

	watcher.setEnableRaisingEvents(true); //TODO: cambiar, no está bueno este diseño. Crear un método Start.
	watcher.startMonitoring();


	std::cin.get();

	return 0;
}
