#ifndef ImplementationSelector_h__
#define ImplementationSelector_h__

//#include <boost/config.hpp>
#include "FileSystemMonitorGenericImpl.hpp"

//namespace detail
//{

// #ifdef BOOST_WINDOWS
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__) 
#include "LinuxImpl.hpp"
#elif defined(__FreeBSD__) // || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#include "FreeBSDImpl.hpp
#elif defined(__CYGWIN__)
#include "UnsupportedImpl.hpp
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include "WinNT32Impl.hpp"
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#include "MacOSXImpl.hpp
#else
#include "UnsupportedImpl.hpp
#endif

namespace detail
{
	typedef FileSystemMonitorGenericImpl<PlatformImpl> ImplementationType;
}



//}


#endif // ImplementationSelector_h__
