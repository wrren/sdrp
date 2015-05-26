/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/

#include <SDRP/Routing/Route.h>

namespace Radicle { namespace SDRP
{
	const RDUInt8 Route::MaxHops = 255;

	Route::Route() :
	m_server( RD_SDRP_UNSPECIFIED_ADDRESS ),
	m_nextHop( RD_SDRP_UNSPECIFIED_ADDRESS ),
	m_hops( 0 ),
	m_age( 0 )
	{}
	
	Route::Route(	const RDNetworkAddress server,
			const RDNetworkAddress nextHop,
			const BloomFilter& services,
			const RDUInt8 hops ) :
	m_server( server ),
	m_nextHop( nextHop ),
	m_services( services ),
	m_hops( hops ),
	m_age( Logger::Time() )
	{}
	
	const RDNetworkAddress Route::Server() const
	{
		return m_server;
	}
	
	void Route::Server( const RDNetworkAddress var )
	{
		m_server = var;
	}
	
	const RDNetworkAddress Route::NextHop() const
	{
		return m_nextHop;
	}
	
	void Route::NextHop( const RDNetworkAddress var )
	{
		m_nextHop = var;
	}
	
	const BloomFilter& Route::Services() const
	{
		return m_services;
	}
	
	void Route::Services( const BloomFilter& var )
	{
		m_services = var;
	}
	
	const RDUInt8 Route::Hops() const
	{
		return m_hops;
	}
	
	void Route::Hops( const RDUInt8 var )
	{
		m_hops = var;
	}
	
	const RDTimeStamp Route::Age() const
	{
		return m_age;
	}
	
	void Route::Age( const RDTimeStamp var )
	{
		m_age = var;
	}
	
	bool Route::operator==( const Route& other ) const
	{
		return m_server == other.m_server && m_nextHop == other.m_nextHop;
	}
	
	bool Route::operator>( const Route& other ) const
	{
		return m_hops > other.m_hops;
	}
	
	bool Route::operator<( const Route& other ) const
	{
		return !( *this > other );
	}
} }

