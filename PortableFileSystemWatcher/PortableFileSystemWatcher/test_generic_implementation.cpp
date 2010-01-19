// TODO: LEER: URGENTE
// http://msdn.microsoft.com/en-us/library/aa383745(VS.85).aspx
// http://blogs.msdn.com/oldnewthing/archive/2007/04/11/2079137.aspx
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
// http://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive


// WINDOWS VERSION AT RUNTIME
//		http://support.microsoft.com/kb/189249
//		http://msdn.microsoft.com/en-us/library/ms724439(VS.85).aspx
//		http://msdn.microsoft.com/en-us/library/ms724832(VS.85).aspx
//		http://msdn.microsoft.com/en-us/library/ms724451(VS.85).aspx
//      http://msdn.microsoft.com/en-us/library/ms725492(VS.85).aspx
//		http://msdn.microsoft.com/en-us/library/ms724833(VS.85).aspx


// LINUX VERSION AT RUNTIME
//		http://linux.die.net/man/2/uname
//		cat /proc/sys/kernel/osrelease

// FREEBSD VERSION AT RUNTIME
//		PROBABLEMENTE: http://linux.die.net/man/2/uname




#include <iostream>
#include <string>

#include <boost/config.hpp>

//#include "FileSystemMonitorImplTestGeneric.hpp"
#include "FileSystemMonitorGeneric.hpp"


//#include "windows.h"
//#include "winuser.h"
// c:\Program Files\Microsoft SDKs\Windows\v6.0A\Include\sdkddkver.h


//#include <boost/asio/detail/push_options.hpp>
//
//#include <boost/asio/detail/push_options.hpp>
//#include <boost/config.hpp>
//#include <boost/system/system_error.hpp>
//#include <boost/asio/detail/pop_options.hpp>
//
//#include <boost/asio/error.hpp>
//#include <boost/asio/detail/noncopyable.hpp>
//#include <boost/asio/detail/socket_types.hpp>
//
//#include <boost/asio/detail/push_options.hpp>
//#include <boost/throw_exception.hpp>
//#include <memory>
//#include <process.h>
//#include <boost/asio/detail/pop_options.hpp>



#ifdef BOOST_WINDOWS
#define PLATFORM_IMPL_CLASS Win32Impl
typedef Win32Impl PlatformImpl;
#endif // BOOST_WINDOWS

//#define PLATFORM LinuxImpl
//#define PLATFORM FreeBSDImpl
//#define PLATFORM MacOSXImpl



int main(int argc, char** argv)
{
	//FileSystemMonitorGenericImpl<PLATFORM_IMPL_CLASS> monitor;
	//FileSystemMonitorGenericImpl<PlatformImpl> monitor;
	//monitor.start();
	FileSystemMonitorGeneric monitor;
	monitor.start();


	//_WIN32_WINNT
	std::cout << "BOOST_WINDOWS: '" << BOOST_WINDOWS << "'" << std::endl;
	std::cout << "_WIN32: '" << _WIN32 << "'" << std::endl;

	//std::cout << "_WIN32_WINNT: '" << _WIN32_WINNT << "'" << std::endl;
	//std::cout << "WINVER: '" << WINVER << "'" << std::endl;

#if defined(WINVER)
	std::cout << "_WIN32_WINNT: '" << _WIN32_WINNT << "'" << std::endl;
	std::cout << "WINVER: '" << WINVER << "'" << std::endl;
#endif


	//std::cout << "__WIN32__: '" << __WIN32__ << "'" << std::endl;
	//std::cout << "WIN32: '" << WIN32 << "'" << std::endl;

	//#elif defined() || defined() || defined(WIN32)
	
	std::cin.get();
	return 0;
}
