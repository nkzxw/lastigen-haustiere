#ifndef Win32ApiWrapper_h__
#define Win32ApiWrapper_h__

#include <string>

#include "windows.h"


namespace Win32ApiWrapper
{
	//WINBASEAPI BOOL WINAPI CreateDirectoryA(__in     LPCSTR lpPathName, /*__in_opt*/ LPSECURITY_ATTRIBUTES lpSecurityAttributes );
	int CreateDirectory ( std::string pathName, LPSECURITY_ATTRIBUTES securityAttributes )
	{
		return ::CreateDirectoryA(pathName.c_str(), securityAttributes);
	}

}



#endif // Win32ApiWrapper_h__