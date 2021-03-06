#pragma once

#include "constants.h"

namespace xo
{
	/// compare floating point variables using relaxed_epsilon
	template< typename T > bool equal( T v1, T v2, T e = constants<T>::ample_epsilon() )
	{ return std::abs( v1 - v2 ) <= std::abs( v1 ) * e; }

	/// compare floating point variables using relaxed_epsilon
	template< typename T > bool less_or_equal( T v1, T v2, T e = constants<T>::ample_epsilon() )
	{ return v1 < v2 || equal( v1, v2, e ); }

	/// compare floating point variables using relaxed_epsilon
	template< typename T > bool greater_or_equal( T v1, T v2, T e = constants<T>::ample_epsilon() )
	{ return v1 > v2 || equal( v1, v2, e ); }
}
