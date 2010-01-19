#ifndef FileSystemMonitorImplTestGeneric_h__
#define FileSystemMonitorImplTestGeneric_h__

struct Win32Impl
{
	static void start()
	{
		std::cout << "Win32Impl::start()" << std::endl;
	}
};

struct LinuxImpl
{
	static void start()
	{
		std::cout << "LinuxImpl::start()" << std::endl;
	}
};

struct FreeBSDImpl
{
	static void start()
	{
		std::cout << "FreeBSDImpl::start()" << std::endl;
	}
};

struct MacOSXImpl
{
	static void start()
	{
		std::cout << "MacOSXImpl::start()" << std::endl;
	}
};

template <typename Platform>
class FileSystemMonitorGenericImpl
{
public:
	void start()
	{
		Platform::start();
	}
};

#endif // FileSystemMonitorImplTestGeneric_h__