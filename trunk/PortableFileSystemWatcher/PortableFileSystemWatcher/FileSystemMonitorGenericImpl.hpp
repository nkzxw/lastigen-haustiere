#ifndef FileSystemMonitorImplTestGeneric_h__
#define FileSystemMonitorImplTestGeneric_h__

namespace detail
{

template <typename PlatformImpl>
class FileSystemMonitorGenericImpl : public PlatformImpl
{
public:

};

}


////TODO: hacemos esto o especializamos???????????
//template <>
//class FileSystemMonitorGenericImpl<Win32Impl> : public PlatformImpl
//{
//public:
//
//};

#endif // FileSystemMonitorImplTestGeneric_h__