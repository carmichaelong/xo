#pragma once

#include "storage.h"

namespace xo
{
	template< typename T, typename L = std::string >
	class file_storage : public storage< T, L >
	{
	public:
		file_storage( const path& filename ) : storage(), filename_( filename )  {}
		virtual ~file_storage() { 
			if ( !empty() )
				std::ofstream( filename_.str() ) << *this;
		}
	private:
		path filename_;
	};

	template< typename T, typename L  > index_t	find_frame_index( const storage<T, L>& sto, const T& value, index_t channel = 0 ) {
		auto l = [&]( const storage<T, L>::const_frame& a, const storage<T, L>::const_frame& b ) { return a[ channel ] < b[ channel ]; };
		auto it = std::lower_bound( sto.begin(), sto.end(), value, l );
		return it.frame_idx_;
	}


	template< typename T, typename L  > void write( storage<T, L>& sto, const string& str, const vec3_<T>& v ) {
		auto f = sto.back();
		f[ str + ".x" ] = v.x;
		f[ str + ".y" ] = v.y;
		f[ str + ".z" ] = v.z;
	}

	template< typename T, typename L  > void write( storage<T, L>& sto, const string& str, const quat_<T>& q ) {
		auto f = sto.back();
		f[ str + ".w" ] = q.w;
		f[ str + ".x" ] = q.x;
		f[ str + ".y" ] = q.y;
		f[ str + ".z" ] = q.z;
	}

	template< typename T > void read( typename const storage<T>::const_frame& f, const string& str, vec3_<T>& v ) {
		v.x = f[ str + ".x" ];
		v.y = f[ str + ".y" ];
		v.z = f[ str + ".z" ];
	}

	template< typename T > void read( typename const storage<T>::const_frame& f, const string& str, quat_<T>& q ) {
		q.w = f[ str + ".w" ];
		q.x = f[ str + ".x" ];
		q.y = f[ str + ".y" ];
		q.z = f[ str + ".z" ];
	}
}
