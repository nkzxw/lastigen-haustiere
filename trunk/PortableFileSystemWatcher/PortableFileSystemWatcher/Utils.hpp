#ifndef Utils_h__
#define Utils_h__

#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace utils
{

bool directoryExists( const std::string& path )
{
	boost::filesystem::path fullPath( path, boost::filesystem::native );

	if ( !boost::filesystem::exists( fullPath ) )
	{
		return false;
	}

	if ( !boost::filesystem::is_directory( fullPath ) )
	{
		return false;
	}

	return true;
}

}

#endif // Utils_h__
