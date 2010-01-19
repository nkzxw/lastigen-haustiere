#ifndef FileSystemMonitorGeneric_h__
#define FileSystemMonitorGeneric_h__

#include "ImplementationSelector.hpp"

class FileSystemMonitorGeneric
{
public:
	FileSystemMonitorGeneric()
		: implementation_( new detail::ImplementationType )
	{}

	void startMonitoring()
	{
		implementation_->startMonitoring("");
	}

private:
	detail::ImplementationType* implementation_;
};

#endif // FileSystemMonitorGeneric_h__