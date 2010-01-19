#ifndef FileSystemMonitorGeneric_h__
#define FileSystemMonitorGeneric_h__

#include "FileSystemMonitorGenericImpl.hpp"

class FileSystemMonitorGeneric
{
public:
	FileSystemMonitorGeneric()
		: implementation_( new FileSystemMonitorGenericImpl )
	{}

	void start()
	{
		implementation_->start();
	}

private:
	FileSystemMonitorGenericImpl * implementation_;
};

#endif // FileSystemMonitorGeneric_h__