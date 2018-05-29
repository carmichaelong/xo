#pragma once

#include "xo/string/string_tools.h"

namespace xo
{
	template< typename ID, typename T = uint32_t >
	struct handle
	{
		handle() : value( T( -1 ) ) {}
		explicit handle( T v ) : value( v ) {}
		explicit handle( size_t v ) : value( T( v ) ) { xo_error_if( v < min<T>() || v >= max<T>(), "handle cannot hold value " + to_str( v ) ); }

		explicit operator bool() const { return value != T( -1 ); }
		explicit operator T() const { return value; }

		friend bool operator==( const handle a, const handle b ) { return a.value == b.value; }
		friend bool operator!=( const handle a, const handle b ) { return a.value != b.value; }

		T value;
	};
}