#ifndef UnsupportedImpl_h__
#define UnsupportedImpl_h__

namespace detail
{

struct UnsupportedImpl
{
	void start()
	{
		std::cout << "UnsupportedImpl::start()" << std::endl;
	}
};

typedef UnsupportedImpl PlatformImpl;

}

#endif // UnsupportedImpl_h__
