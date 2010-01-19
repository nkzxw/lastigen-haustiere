#ifndef FreeBSDImpl_h__
#define FreeBSDImpl_h__

namespace detail
{

struct FreeBSDImpl
{
	void start()
	{
		std::cout << "FreeBSDImpl::start()" << std::endl;
	}
};

typedef FreeBSDImpl PlatformImpl;

}

#endif // FreeBSDImpl_h__