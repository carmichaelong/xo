#pragma once

#include "mat33_type.h"
#include "vec3_type.h"

namespace xo
{
	template< typename T >
	mat33_<T> mat33_from_axes( const vec3_<T>& x, const vec3_<T>& y, const vec3_<T>& z )
	{ return mat33_<T>( x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z ); }

	template< typename T >
	mat33_<T> mat33_from_diagonal( const vec3_<T>& d )
	{ return mat33_<T>( d.x, T(0), T( 0 ), T( 0 ), d.y, T( 0 ), T( 0 ), T( 0 ), d.z ); }
}
