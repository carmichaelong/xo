#pragma once

#include <iosfwd>
#include "xo/string/string_type.h"
#include "xo/system/xo_api.h"
#include "xo/string/string_cast.h"

#ifdef XO_COMP_MSVC
#	pragma warning( push )
#	pragma warning( disable: 4251 )
#endif

namespace std {
	namespace filesystem {
		class path;
	}
}

namespace xo
{
	class XO_API path
	{
	public:
		path() {}
		path( const path& p ) : data_( p.data_ ) {}
		path( path&& p ) : data_( std::move( p.data_ ) ) {}
		path( const char* p ) : data_( p ) {}
		path( std::filesystem::path& p );
		explicit path( const string& p ) : data_( p ) {}
		explicit path( string&& p ) : data_( std::move( p ) ) {}

		path& operator=( const path& p ) { data_ = p.data_; return *this; }
		path& operator=( path&& p ) { data_ = std::move( p.data_ ); return *this; }
		path& operator=( const string& p ) { data_ = p; return *this; }
		path& operator=( string&& p ) { data_ = std::move( p ); return *this; }

		static char preferred_separator();

		const string& string() const { return data_; }
		const char* c_str() const { return data_.c_str(); }

		path& replace_extension( const path& ext = path() );
		path& remove_filename();
		path& replace_filename( const path& f = path() );
		path& make_preferred();
		path parent_path() const;
		path extension() const;
		path filename() const;
		path stem() const;
		bool empty() const;
		path& operator/=( const path& p );
		path& operator/=( const xo::string& p );

		bool has_filename() const;
		bool has_parent_path() const;

		static const size_t npos;

	private:
		size_t last_separator_pos() const;
		std::string data_;
	};

	XO_API path operator/( const path& p1, const path& p2 );
	XO_API path operator/( const path& p1, const string& p2 );
	XO_API path operator/( const path& p1, const char* p2 );
	XO_API path operator+( const path& p1, const string& p2 );
	XO_API path operator+( const string& p1, const path& p2 );
	XO_API bool operator==( const path& p1, const path& p2 );
	XO_API bool operator!=( const path& p1, const path& p2 );

	inline std::ostream& operator<<( std::ostream& str, const path& p ) { return str << p.string(); }
	inline bool operator<( const path& p1, const path& p2 ) { return p1.string() < p2.string(); }

	template<> struct string_cast< path > {
		static path from( const string& s ) { return path( s ); }
		static string to( const path& v ) { return v.string(); }
	};
}

#ifdef XO_COMP_MSVC
#	pragma warning( pop )
#endif
