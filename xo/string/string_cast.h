#pragma once

#include "xo/system/xo_api.h"
#include "xo/system/assert.h"
#include "xo/string/string_type.h"

#include <type_traits>
#include <sstream> // #TODO: really should get rid of this header

namespace xo
{
	/// convert any type to string
	template< typename T > string to_str( const T& value );
	template< typename T > T from_str( const string& s, T default_value );

	template< typename T, typename E = void > struct string_cast;

	template<> struct XO_API string_cast<float>
	{
		static string to( const float& value );
		static float from( const string& s );
	};

	template<> struct XO_API string_cast<double>
	{
		static string to( const double& value );
		static double from( const string& s );
	};

	template<> struct XO_API string_cast<bool>
	{
		static string to( const bool& value );
		static bool from( const string& s );
	};

	template<> struct XO_API string_cast<long>
	{
		static string to( const long& value );
		static long from( const string& s );
	};

	template<> struct XO_API string_cast<unsigned long>
	{
		static string to( const unsigned long& value );
		static unsigned long from( const string& s );
	};

	template<> struct XO_API string_cast<int>
	{
		static string to( const int& value );
		static int from( const string& s );
	};

	template<> struct XO_API string_cast<unsigned long long>
	{
		static string to( const unsigned long long& value );
		static unsigned long long from( const string& s );
	};

	template<> struct XO_API string_cast<string>
	{
		static string to( const string& value ) { return value; }
		static string from( const string& s ) { return s; }
	};

	template<> struct XO_API string_cast<const char*>
	{
		static string to( const char*& value ) { return string( value ); }
		static const char* from( const string& s ) { return s.c_str(); }
	};

	template< typename T > struct string_cast< T, typename std::enable_if< std::is_enum< T >::value >::type > {
		static T from( const string& s ) { return static_cast< T >( string_cast< int >::from( s ) ); }
		static string to( const T& v ) { return string_cast< int >::to( static_cast< int >( v ) ); }
	};

	/// make string out of container of elements
	template< typename C > string to_str( const C& c, const string& delim ) {
		string s;
		for ( auto it = std::begin( c ); it != std::end( c ); ++it ) {
			if ( it != c.begin() ) s += delim;
			s += to_str( *it );
		}
		return s;
	}

	template< typename T > string to_str( T& value ) {
		return string_cast<T>::to( value );
	}

	template< typename T > T from_str( const string& s, T default_value ) {
		try { return string_cast<T>::from( s ); }
		catch ( std::exception& ) { return default_value; }
	}
}
