#include "filesystem.h"

#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "xo/string/string_tools.h"
#include "xo/system/assert.h"

#ifdef XO_COMP_MSVC
#	include <conio.h>
#	include <shlobj.h>
#   include <direct.h>
#	pragma warning( disable: 4996 )
#else
#   include <unistd.h>
#endif

#ifdef __APPLE__
#	include <mach-o/dyld.h>
#endif


namespace xo
{
#ifdef XO_COMP_MSVC
	// define this helper function for windows, since it's so complicated
	path get_known_windows_folder( KNOWNFOLDERID id )
	{
		// get the string, convert to single byte string, then free the original string (ugh)
		wchar_t* wcsLocalAppData = 0;
		SHGetKnownFolderPath( id, 0, NULL, &wcsLocalAppData );
		char mbsLocalAppData[ MAX_PATH ];
		wcstombs_s( size_t(), mbsLocalAppData, MAX_PATH, wcsLocalAppData, MAX_PATH );
		CoTaskMemFree( static_cast<void*>( wcsLocalAppData ) );
		return path( mbsLocalAppData );
	}
#endif

	path get_config_folder()
	{
#ifdef XO_COMP_MSVC
		return get_known_windows_folder( FOLDERID_LocalAppData );
#else
		string homeDir = std::getenv( "HOME" );
		return path{homeDir + "/.config"};
#endif
	}

	path get_documents_folder()
	{
#ifdef XO_COMP_MSVC
		return get_known_windows_folder( FOLDERID_Documents );
#else
		return std::getenv( "HOME" );
#endif
	}

	path get_application_folder()
	{
#ifdef XO_COMP_MSVC
		char buf[ 1024 ];
		GetModuleFileName( 0, buf, sizeof( buf ) );
		return path( buf ).parent_path();
#elif defined(__linux__)
		char buf[ 1024 ];
		readlink("/proc/self/file", buf, sizeof( buf ));
		return path( buf ).parent_path();
#elif defined(__APPLE__)
		uint32_t bufferSize = 1024;
		std::vector<char> buf(bufferSize + 1);
		if (_NSGetExecutablePath(buf.data(), &bufferSize))
		{
			buf.resize(bufferSize);
			_NSGetExecutablePath(buf.data(), &bufferSize);
		}
		return path( buf.data() ).parent_path();
#else
		return "";
#endif
	}

	XO_API path temp_directory_path()
	{
		return std::getenv( "TMP" );
	}

	XO_API bool copy_file( const path& from, const path& to, bool overwrite )
	{
#ifdef XO_COMP_MSVC
		return CopyFile( from.c_str(), to.c_str(), overwrite ) == TRUE;
#else
		if ( overwrite || !file_exists( to ) )
		{
			std::ifstream src( from.string(), std::ios::binary );
			std::ofstream dst( to.string(), std::ios::binary );
			if ( src.good() && dst.good() )
			{
				dst << src.rdbuf();
				return true;
			}
		}
		return false;
#endif
	}

	XO_API bool exists( const path& p )
	{
#ifdef XO_COMP_MSVC
		return GetFileAttributes( path( p ).make_preferred().c_str() ) != INVALID_FILE_ATTRIBUTES;
#else
		struct stat info;
		return stat( p.c_str(), &info ) == 0;
#endif
	}

	XO_API bool file_exists( const path& file )
	{
		std::ifstream ifs( file.string() );
		return ifs.good();
	}

	XO_API bool folder_exists( const path& folder )
	{
#ifdef XO_COMP_MSVC
		DWORD dwAttrib = GetFileAttributes( path( folder ).make_preferred().c_str() );
		return ( dwAttrib != INVALID_FILE_ATTRIBUTES && ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
#else
		struct stat status;
		if ( stat( folder.c_str(), &status ) == 0 )
		{
			if ( status.st_mode & S_IFDIR )
				return true;
		}
		return false; // if any condition fails
#endif
	}

	xo::optional< xo::path > try_find_file( std::initializer_list< path > filenames )
	{
		for ( auto& f : filenames )
			if ( file_exists( f ) )
				return f;
		return xo::optional< xo::path >();
	}

	XO_API path find_file( std::initializer_list< path > filenames )
	{
		for ( auto& f : filenames )
			if ( file_exists( f ) )
				return f;

		string names;
		for ( auto& p : filenames )
			names += p.string() + " ";

		xo_error( "Could not find " + names + " in " + xo::current_path().string() );
	}

	XO_API bool create_directories( const path& folder )
	{
		// make sure parent folders exist
		if ( folder.has_parent_path() && !folder_exists( folder.parent_path() ) )
			create_directories( folder.parent_path() );

#ifdef XO_COMP_MSVC
		return CreateDirectory( folder.c_str(), NULL ) != 0;
#else
		return mkdir( folder.c_str(), 0777 ) == 0;
#endif
	}

	XO_API path create_unique_folder( const path& folder, int max_attempts )
	{
		path unique_folder = folder;
		bool success = false;
		for ( int i = 0; i < max_attempts && !success; ++i )
		{
			if ( i > 0 )
				unique_folder = folder + stringf( " (%d)", i );

			if ( !folder_exists( unique_folder ) )
				success = create_directories( unique_folder ); // try to create folder
		}
		xo_error_if( !success, "Could not create unique folder after " + to_str( max_attempts ) + " attempts" );

		return unique_folder;
	}

	XO_API string load_string( const path& filename, error_code* ec )
	{
		// this method uses a stringbuf, which requires an extra copy (C++ suckiness)
		std::ifstream ifstr( filename.string() );
		if ( !ifstr.good() )
			return set_error_or_throw( ec, "Could not open " + filename.string() ), "";

		std::stringstream buf;
		buf << ifstr.rdbuf();
		return buf.str();
	}

	XO_API bool current_path( const path& p )
	{
		return chdir( p.c_str() ) == 0;
	}

	XO_API path current_path()
	{
		char* buf = getcwd( NULL, 0 );
		path p( buf );
		free( buf );
		return p;
	}

	XO_API std::time_t last_write_time( const path& p )
	{
#ifdef XO_COMP_MSVC
#	ifdef _USE_32BIT_TIME_T
		struct _stat32 result;
		if ( _stat32( p.c_str(), &result ) == 0 )
			return result.st_mtime;
		else xo_error( "Could not query " + p.string() );
#	else
		struct _stat64 result;
		if ( _stat64( p.c_str(), &result ) == 0 )
			return result.st_mtime;
		else xo_error( "Could not query " + p.string() );
#	endif

#else
	struct stat attr;
	if ( stat(p.c_str(), &attr) == 0 )
		return attr.st_mtime;
	else
		xo_error( "Could not query " + p.string() );
#endif
	}
}
