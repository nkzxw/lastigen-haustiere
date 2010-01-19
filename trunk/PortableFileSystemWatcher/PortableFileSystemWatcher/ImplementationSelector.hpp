#ifndef ImplementationSelector_h__
#define ImplementationSelector_h__

#include "FileSystemMonitorGenericImpl.hpp"
#include "WinNT32Impl.hpp"

typedef FileSystemMonitorGenericImpl<Win32Impl> ImplementationType;

#endif // ImplementationSelector_h__
