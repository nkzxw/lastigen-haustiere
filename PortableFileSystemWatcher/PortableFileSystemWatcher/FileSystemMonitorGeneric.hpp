#ifndef FileSystemMonitorGeneric_h__
#define FileSystemMonitorGeneric_h__

#include "ImplementationSelector.hpp"

class FileSystemMonitorGeneric
{
public:
	FileSystemMonitorGeneric()
		: implementation_( new ImplementationType )
	{}

	void start()
	{
		implementation_->start();
	}

private:
	ImplementationType* implementation_;
};

#endif // FileSystemMonitorGeneric_h__