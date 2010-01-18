#ifndef NotifyFilters_h__
#define NotifyFilters_h__

//enum NotifyFilters
namespace NotifyFilters
{
	static const int FileName		= 0x00000001;  //1;
	static const int DirectoryName	= 0x00000002;  //2;
	static const int Attributes		= 0x00000004;  //4;
	static const int Size			= 0x00000008;  //8;
	static const int LastWrite		= 0x00000010;  //16;
	static const int LastAccess		= 0x00000020;  //32;
	static const int CreationTime	= 0x00000040;  //64;
	static const int Security		= 0x00000100;  //256;
}

#endif // NotifyFilters_h__