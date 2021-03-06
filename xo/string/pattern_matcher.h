#pragma once

#include <vector>
#include "string_tools.h"
#include "xo/container/prop_node.h"

namespace xo
{
	class pattern_matcher
	{
	public:
		pattern_matcher() {}
		pattern_matcher( const prop_node& pn ) : patterns( split_str( pn.get<string>(), ";" ) ) {}
		pattern_matcher( const char* pattern, const char* delimeters = ";" ) : patterns( split_str( pattern, delimeters ) ) {}
		pattern_matcher( const string& pattern, const char* delimeters = ";" ) : patterns( split_str( pattern, delimeters ) ) {}

		// returns true if pattern matches string
		bool operator()( const string& str ) const {
			for ( auto& p : patterns )
				if ( pattern_match( str, p ) ) return true;
			return false;
		}

		template< typename T > bool operator()( const T& str ) const { return operator()( static_cast< const string& >( *str ) ); }
		template< typename T > bool operator()( const T* str ) const { return operator()( static_cast< const string& >( *str ) ); }

	private:
		std::vector< string > patterns;
	};

	inline bool is_pattern( const string& s ) { return s.find_first_of( "*?" ) != string::npos; }

	inline bool from_prop_node( const prop_node& pn, pattern_matcher& v ) { v = pattern_matcher( pn ); return true; }
}
