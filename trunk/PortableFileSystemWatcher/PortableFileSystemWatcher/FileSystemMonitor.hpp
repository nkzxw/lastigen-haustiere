#ifndef FileSystemMonitor_h__
#define FileSystemMonitor_h__

#include <string>

#include <boost/function.hpp>


//enum NotifyFilters
namespace NotifyFilters
{
	static const int FileName = 1;
	static const int DirectoryName = 2;
	static const int Attributes = 4;
	static const int Size = 8;
	static const int LastWrite = 16;
	static const int LastAccess = 32;
	static const int CreationTime = 64;
	static const int Security = 256;
}

//TODO;
struct FileSystemEventArgs
{
	std::string Name;
};


//TODO;
struct RenamedEventArgs
{
	std::string Name;
};

//Similar a un C# Delegate
typedef boost::function<void (FileSystemEventArgs e)> FileSystemEventHandler;
typedef boost::function<void (RenamedEventArgs e)> RenamedEventHandler;

class FileSystemMonitor
{
public:
	FileSystemMonitor()
	{
	}

	std::string getPath() const //TODO: evitar copias
	{ 
		return path_; 
	}
	void setPath (const std::string& val) 
	{ 
		//TODO: debe chequear el Path que exista en el FileSystem...
		path_ = val; 
	}

	int getNotifyFilters() const //TODO: evitar copias
	{ 
		return notifyFilters_; 
	}
	void setNotifyFilters(int val) 
	{ 
		notifyFilters_ = val; 
	}

	std::string getFilter() const //TODO: evitar copias
	{ 
		return filter_; 
	}
	void setFilter(const std::string& val) 
	{ 
		filter_ = val; 
	}

	bool getEnableRaisingEvents() const 
	{ 
		return enableRaisingEvents_; 
	}
	void setEnableRaisingEvents(bool val) 
	{ 
		enableRaisingEvents_ = val; 
	}

	// Event Handlers
	FileSystemEventHandler Changed;
	FileSystemEventHandler Created;
	FileSystemEventHandler Deleted;
	RenamedEventHandler Renamed;

	//TODO: eliminar, esto debe ser autogenerado
	void RaiseChanged()
	{
		if (Changed)
		{
			FileSystemEventArgs temp;
			temp.Name = "nombre_del_archivo.ext";

			Changed(temp);
		}
	}

	//TODO: eliminar, esto debe ser autogenerado
	void RaiseCreated()
	{
		if (Created)
		{
			FileSystemEventArgs temp;
			temp.Name = "nombre_del_archivo.ext";

			Created(temp);
		}
	}

	//TODO: eliminar, esto debe ser autogenerado
	void RaiseDeleted()
	{
		if (Deleted)
		{
			FileSystemEventArgs temp;
			temp.Name = "nombre_del_archivo.ext";

			Deleted(temp);
		}
	}

	//TODO: eliminar, esto debe ser autogenerado
	void RaiseRenamed()
	{
		if (Renamed)
		{
			//FileSystemEventArgs temp;
			RenamedEventArgs temp;
			temp.Name = "nombre_del_archivo.ext";

			Renamed(temp);
		}
	}

private:
	std::string path_;
	int notifyFilters_; //TODO: debería ser un enum
	std::string filter_;
	bool enableRaisingEvents_;


};

#endif // FileSystemMonitor_h__