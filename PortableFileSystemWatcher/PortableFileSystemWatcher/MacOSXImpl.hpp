#ifndef MacOSXImpl_h__
#define MacOSXImpl_h__

namespace detail
{

struct MacOSXImpl
{
	void start()
	{
		std::cout << "MacOSXImpl::start()" << std::endl;
	}
};

typedef MacOSXImpl PlatformImpl;

}

#endif // MacOSXImpl_h__