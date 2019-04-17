#pragma once

#include "xo/xo_types.h"
#include "xo/system/assert.h"
#include "xo/system/system_tools.h"
#include "xo/string/string_type.h"
#include "xo/string/string_cast.h"
#include "xo/string/string_tools.h"
#include "xo/utility/optional.h"
#include "xo/container/view_if.h"

#include <vector>
#include <utility>
#include <initializer_list>

namespace xo
{
	/// prop_node class
	class XO_API prop_node
	{
	public:
		using key_t = string;
		using value_t = string;
		using pair_t = std::pair< key_t, prop_node >;
		using container_t = std::vector< pair_t >;
		using iterator = container_t::iterator;
		using const_iterator = container_t::const_iterator;

		/// constructors
		prop_node() : accessed_flag( false ) {}
		prop_node( const prop_node& other ) = default;
		prop_node( prop_node&& other ) = default;
		prop_node( const char* pn );
		prop_node( const value_t& v ) : accessed_flag( false ), value( v ) {}
		prop_node( value_t&& v ) : accessed_flag( false ), value( std::move( v ) ) {}

		/// destructor (non-virtual)
		~prop_node() {}

		/// assignment operators
		prop_node& operator=( const prop_node& other ) = default;
		prop_node& operator=( prop_node&& other ) = default;
<<<<<<< HEAD
		template< typename T > prop_node& operator=( const T& v );
=======
		template< typename T > prop_node& operator=( const T& v ) { return set( v ); }
>>>>>>> master

		/// equality operators
		bool operator==( const prop_node& other ) const;
		bool operator!=( const prop_node& other ) const { return !( *this == other ); }

		/// get the value of this node, throws if conversion fails
		template< typename T > T get() const;

		/// get the value of a child node, throws if value does not exist
		template< typename T > T get( const key_t& key ) const;

		/// get the value of a child node, throws if value does not exist
		template< typename T > T get( index_t idx ) const;

		/// get the value of a child node, or a default value if it doesn't exist
		template< typename T > T get( const key_t& key, const T& def ) const;

		/// get an optional value of a child node
		template< typename T > optional<T> try_get( const key_t& key ) const;

		/// get a value of a child node, only stores if exists
		template< typename T > bool try_get( T& value, const key_t& key ) const;

		/// get the value of a child node for a range of keys, or a default value if it doesn't exist
		template< typename T > T get_any( std::initializer_list< key_t > keys, const T& def ) const;

		/// get the value of a child node for a range of keys, or a default value if it doesn't exist
		template< typename T > T get_any( std::initializer_list< key_t > keys ) const;

		/// get the value of a child node for a range of keys, or a default value if it doesn't exist
		template< typename T > optional< T > try_get_any( std::initializer_list< key_t > keys ) const;

		/// set the value of this node
		prop_node& set( prop_node&& pn ) { return *this = std::move( pn ); }
		prop_node& set( const prop_node& pn ) { return *this = pn; }
		template< typename T > prop_node& set( const T& v ) { return *this = to_prop_node( v ); }

		/// set the value of a child node, the node is created if not existing
		template< typename T > prop_node& set( const key_t& key, const T& v );

		/// set the value of this node (does not affect children)
		template< typename T > prop_node& set_value( const T& v ) { value = to_str( v ); return *this; }
		prop_node& set_value( value_t&& v ) { value = std::move( v ); return *this; }

		/// see if this prop_node has a value
		bool has_value() const { return !value.empty(); }
		bool has_value( const key_t& key ) const;

		/// see if this prop_node has a specific key
		bool has_key( const key_t& key ) const;

		/// see if this prop_node has any specific key
		bool has_any_key( std::initializer_list< key_t > keys ) const;

		/// number of direct child keys
		size_t size() const { return children.size(); }

		/// number of child layers
		size_t count_layers() const;

		/// number of children (recursively)
		size_t count_children() const;

		/// returns true if all keys are empty
		bool is_array() const;

		/// true if prop_node has a value or a key
		bool empty() const { return value.empty() && children.empty(); }

		/// clear value and keys
		void clear() { value.clear(); children.clear(); }

		/// get raw value_t reference, does not set access flag
		const value_t& raw_value() const { return value; }

<<<<<<< HEAD
		/// set the value of this node
		prop_node& set( prop_node&& pn ) { *this = std::move( pn ); return *this; }
		prop_node& set( const prop_node& pn ) { *this = pn; return *this; }
		template< typename T > prop_node& set( const T& v );

		/// set the value of this node
		template< typename T > prop_node& set_value( const T& v ) { value = to_str( v ); return *this; }
		prop_node& set_value( value_t&& v ) { value = std::move( v ); return *this; }

		/// set the value of a child node, the node is created if not existing
		template< typename T > prop_node& set( const key_t& key, const T& v )
		{ if ( auto c = try_get_child( key ) ) return c->set( v ); else return push_back( key, v ); }

=======
>>>>>>> master
		/// set the value of a child node, accessing children through delimiter character
		template< typename T > prop_node& set_query( const key_t& query, const T& v, const char delim = '.' );

		/// add a node with a value
		template< typename T > prop_node& push_back( const key_t& key, const T& value );
<<<<<<< HEAD
		prop_node& push_back( const key_t& key, const prop_node& pn ) { children.emplace_back( key, pn ); return children.back().second; }
		prop_node& push_back( const key_t& key, prop_node&& pn ) { children.emplace_back( key, std::move( pn ) ); return children.back().second; }
		prop_node& push_back( const key_t& key ) { children.emplace_back( key, prop_node() ); return children.back().second; }
=======
		prop_node& push_back( const key_t& key, const prop_node& pn );
		prop_node& push_back( const key_t& key, prop_node&& pn );
		prop_node& push_back( const key_t& key );
>>>>>>> master

		/// insert children
		iterator insert( iterator pos, const_iterator first, const_iterator last );

		// append children from another prop_node
		iterator append( const prop_node& other );
		iterator append( prop_node&& other );
		void merge( const prop_node& other, bool overwrite = false );

		/// reserve children
		void reserve( size_t n ) { children.reserve( n ); }

		/// access child by name, throws if not existing
		const prop_node& get_child( const key_t& key ) const;
		const prop_node& get_any_child( std::initializer_list< key_t > keys ) const;
		const prop_node* try_get_any_child( std::initializer_list< key_t > keys ) const;

		const prop_node& operator[]( const key_t& key ) const { return get_child( key ); }
		prop_node& get_child( const key_t& key );

		/// get a child node or add it if not existing
		prop_node& get_or_add_child( const key_t& key );
		prop_node& operator[]( const key_t& key ) { return get_or_add_child( key ); }

		/// get a child node by index, throws if invalid
		const prop_node& get_child( index_t idx ) const;
		prop_node& get_child( index_t idx );
		const prop_node& operator[]( index_t idx ) const { return get_child( idx ); }
		prop_node& operator[]( index_t idx ) { return get_child( idx ); }

		/// get a child node, return nullptr if not existing
		const prop_node* try_get_child( const key_t& key ) const;
		prop_node* try_get_child( const key_t& key );

		/// get a child node using delimiters, return nullptr if not existing
		const prop_node* try_get_query( const key_t& query, const char delim = '.' ) const;
		prop_node* try_get_query( const key_t& query, const char delim = '.' );
		prop_node& get_or_add_query( const key_t& query, const char delim = '.' );

		/// get key by index
		const key_t& get_key( index_t idx ) const;

		/// begin of child nodes
		iterator begin() { access(); return children.begin(); }
		const_iterator begin() const { access(); return children.begin(); }
		const_iterator cbegin() const { access(); return children.cbegin(); }
		const pair_t& front() const { return children.front(); }
		pair_t& front() { return children.front(); }

		/// end of child nodes
		iterator end() { return children.end(); }
		const_iterator end() const { return children.end(); }
		const_iterator cend() const { return children.cend(); }
		const pair_t& back() const { return children.back(); }
		pair_t& back() { return children.back(); }

		/// access selection with specific key
		auto select( const string& key ) const
		{ return make_view_if( begin(), end(), [=]( const pair_t& kvp ) { return kvp.first == key; } ); }

		/// access items with specific pattern
		auto select_pattern( const string& pattern ) const
		{ return make_view_if( begin(), end(), [=]( const pair_t& kvp ) { return pattern_match( kvp.first, pattern ); } ); }

		/// erase a child
		iterator erase( const_iterator it ) { return children.erase( it ); }
		bool erase( const key_t& key );
		bool erase_query( const key_t& query, const char delim = '.' );

		void pop_back() { children.pop_back(); }

		/// see if this node has been accessed
		bool is_accessed() const;
		size_t count_unaccessed() const;
		void set_accessed_recursively( bool access ) const;
		void access() const { accessed_flag = true; }

	private:
		const prop_node* try_get_query_key( const key_t& key ) const;

		mutable bool accessed_flag;
		value_t value;
		container_t children;
	};

	/// convert prop_node to string
	XO_API string to_str( const prop_node& pn );

	/// prop_node conversion
	template< typename T > prop_node to_prop_node( const T& v ) {
		return prop_node( to_str( v ) );
	}

	template< typename T > bool from_prop_node( const prop_node& pn, T& v ) {
		return from_str( pn.raw_value(), v ); 
	};

	template< typename T > T from_prop_node( const prop_node& pn ) {
		if ( T v; from_prop_node( pn, v ) )
			return v;
		else xo_error( "Could not convert prop_node to " + get_type_name<T>() );
	}

	template< typename T > prop_node to_prop_node( const std::vector<T>& vec ) {
		prop_node pn;
		for ( size_t i = 0; i < vec.size(); ++i )
			pn.push_back( "", to_prop_node( vec[ i ] ) );
		return pn;
	}

	template< typename T > bool from_prop_node( const prop_node& pn , std::vector<T>& vec ) {
		vec.resize( pn.size() );
		bool ok = true;
		for ( index_t i = 0; i < pn.size(); ++i )
			ok &= from_prop_node( pn[ i ], vec[ i ] );
		return ok;
	};

	//
	// prop_node class implementations
	//

	template< typename T >
	T prop_node::get() const	{
		access();
		return from_prop_node<T>( *this );
	}

	template< typename T >
	T prop_node::get( const key_t& key ) const {
		return get_child( key ).get< T >();
	}

	template< typename T >
	T prop_node::get( index_t idx ) const {
		return get_child( idx ).get< T >();
	}

	template< typename T >
	T prop_node::get( const key_t& key, const T& def ) const {
		if ( auto c = try_get_child( key ) ) return c->get< T >(); else return def;
	}

	template< typename T >
	optional<T> prop_node::try_get( const key_t& key ) const {
		if ( auto c = try_get_child( key ) )
			return c->get< T >();
		else return optional< T >();
	}

	template< typename T >
	bool prop_node::try_get( T& value, const key_t& key ) const	{
		if ( auto c = try_get_child( key ) )
		{ value = c->get< T >(); return true; }
		else return false;
	}

	template< typename T >
	T prop_node::get_any( std::initializer_list< key_t > keys, const T& def ) const	{
		if ( auto c = try_get_any_child( keys ) )
			return c->get< T >();
		return def;
	}

	template< typename T >
	T prop_node::get_any( std::initializer_list< key_t > keys ) const {
		if ( auto c = try_get_any_child( keys ) )
			return c->get< T >();
		xo_error( "Could not find key: " + concatenate_str( keys, " or " ) );
	}

	template< typename T > optional< T >
	prop_node::try_get_any( std::initializer_list< key_t > keys ) const {
		if ( auto c = try_get_any_child( keys ) )
			return c->get< T >();
		return optional< T >();
	}

	template< typename T >
	prop_node& prop_node::set( const key_t& key, const T& v ) {
		if ( auto c = try_get_child( key ) )
			return c->set( v );
		else return push_back( key, v );
	}

	template< typename T >
	prop_node& prop_node::set_query( const key_t& query, const T& v, const char delim ) {
		return get_or_add_query( query, delim ).set( v );
	}

	template< typename T >
	prop_node& prop_node::push_back( const key_t& key, const T& value ) {
		children.emplace_back( key, to_prop_node( value ) );
		return children.back().second;
	}
}
