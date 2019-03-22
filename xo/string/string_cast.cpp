#include "string_cast.h"

#include <vcruntime_typeinfo.h>

namespace xo
{
	string string_cast<float>::to( const float& v )
	{
		char buf[ 32 ];
		std::snprintf( buf, sizeof( buf ), "%g", v );
		return string( buf );
	}

	string string_cast<double>::to( const double& v )
	{
		char buf[ 32 ];
		std::snprintf( buf, sizeof( buf ), "%g", v );
		return string( buf );
	}

	string string_cast<bool>::to( const bool& v )
	{
		return string( v ? "1" : "0" );
	}

	string string_cast<long>::to( const long& v )
	{
		return std::to_string( v );
	}

	string string_cast<unsigned long>::to( const unsigned long& v )
	{
		return std::to_string( v );
	}

	string string_cast<int>::to( const int& v )
	{
		return std::to_string( v );
	}

	string string_cast<unsigned long long>::to( const unsigned long long& v )
	{
		return std::to_string( v );
	}

	float string_cast<float>::from( const string& s )
	{
		char* p;
		auto v = std::strtof( s.c_str(), &p );
		xo_error_if( p == s.c_str(), "Could not convert string to float" );
		return v;
	}
		
	double string_cast<double>::from( const string& s )
	{
		char* p;
		auto v = std::strtod( s.c_str(), &p );
		xo_error_if( p == s.c_str(), "Could not convert string to double" );
		return v;
	}

	bool string_cast<bool>::from( const string& s )
	{
		return ( s == "1" || s == "true" );
	}

	long string_cast<long>::from( const string& s )
	{
		char* p;
		auto v = std::strtol( s.c_str(), &p, 10 );
		xo_error_if( p == s.c_str(), "Could not convert string to int" );
		return v;
	}

	unsigned long string_cast<unsigned long>::from( const string& s )
	{
		char* p;
		auto v = std::strtoul( s.c_str(), &p, 10 );
		xo_error_if( p == s.c_str(), "Could not convert string to int" );
		return v;
	}

	int string_cast<int>::from( const string& s )
	{
		char* p;
		auto v = std::strtol( s.c_str(), &p, 10 );
		xo_error_if( p == s.c_str(), "Could not convert string to int" );
		return v;
	}

	unsigned long long string_cast<unsigned long long>::from( const string& s )
	{
		char* p;
		auto v = std::strtoull( s.c_str(), &p, 10 );
		xo_error_if( p == s.c_str(), "Could not convert string to int" );
		return v;
	}
}
