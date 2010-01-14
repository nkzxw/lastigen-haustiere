#ifndef Win32ApiWrapper_h__
#define Win32ApiWrapper_h__

#include <string>

#include "windows.h"


namespace Win32ApiWrapper
{
	//WINBASEAPI BOOL WINAPI CreateDirectoryA(__in     LPCSTR lpPathName, /*__in_opt*/ LPSECURITY_ATTRIBUTES lpSecurityAttributes );
	int CreateDirectory ( const std::string& pathName, LPSECURITY_ATTRIBUTES securityAttributes )
	{
		return ::CreateDirectoryA(pathName.c_str(), securityAttributes);
	}


	
		
	void* CreateFile ( const std::string& lpFileName, unsigned long dwDesiredAccess, unsigned long dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, unsigned long dwCreationDisposition, unsigned long dwFlagsAndAttributes, void* hTemplateFile )
	{
		
		#ifdef UNICODE
		//#define CreateFile  CreateFileW
			return ::CreateFileW( lpFileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
		#else
			return ::CreateFileA( lpFileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
		#endif // !UNICODE
	}





}



#endif // Win32ApiWrapper_h__