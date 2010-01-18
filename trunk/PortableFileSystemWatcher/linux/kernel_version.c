#include <stdio.h>
#include <stdlib.h>
#include <linux/version.h>

// TODO: testear en Linux < 2.6 y Win9x
// TODO: testear en freebsd que no soporte

// boost::config 
// BOOST_WINDOWS
// BOOST_LINUX
//  && (!defined(_WIN32_WINNT) || _WIN32_WINNT < 0x0600) \
//# if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0501) || defined(UNDER_CE)
//// Building for Windows XP, Windows Server 2003, or later.
//# if !defined(_WIN32_WINNT) && !defined(_WIN32_WINDOWS)
//#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)

//#if defined(__linux__) // This service is only supported on Linux.

//#if !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__) && !defined(__DragonFly__)
//#error "This platform is not BSD"
//#endif
//#elif defined(__FreeBSD__) && (__FreeBSD__ <= 4) || defined(__osf__)
#if defined(__hpux) || defined(__osf__) || defined(__vms) || (defined(__FreeBSD__) && (__FreeBSD__ < 8)) 

int main( int argc, char **argv ) 
{

#if LINUX_VERSION_CODE >= KERNEL_VERSION (2,6,13) 
#define BOOST_FILESYSTEMMONITOR_HAS_OS_NOTIFICATION 1
	printf( "Kernel Version >= 2.6.13.\n");       
#else
	printf( "Kernel Version < 2.6.13.\n");       
#endif
	exit( 0 );
}
