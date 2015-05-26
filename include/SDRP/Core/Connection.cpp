/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Core/Connection.h>

namespace Radicle { namespace SDRP
{
	Connection::Connection() :
	m_time( 0 )
	{}
	
	Connection::Connection( const RDNetworkAddress address, 
				const RDNetworkAddress nextHop,
				const RDServiceIdentifier service,
				const RDTimeStamp time ) :
	m_address( address ),
	m_nextHop( nextHop ),
	m_service( service ),
	m_time( time )
	{}
	
	const RDTimeStamp Connection::Age() const
	{
		return m_time;
	}
	
	void Connection::Age( const RDTimeStamp age )
	{
		m_time = age;
	}

	const RDNetworkAddress Connection::Address() const
	{
		return m_address;
	}
	
	void Connection::Address( const RDNetworkAddress address )
	{
		m_address = address;
	}

	const RDNetworkAddress Connection::NextHop() const
	{
		return m_nextHop;
	}
	
	void Connection::NextHop( const RDNetworkAddress nextHop )
	{
		m_nextHop = nextHop;
	}

	const RDServiceIdentifier Connection::Service() const
	{
		return m_service;
	}
	
	void Connection::Service( const RDServiceIdentifier service )
	{
		m_service = service;
	}

	bool Connection::operator==( const Connection& other ) const
	{
		return m_address == other.m_address && m_service == other.m_service;
	}

	bool Connection::operator>( const Connection& other ) const
	{
		return m_address > other.m_address;
	}

	bool Connection::operator<( const Connection& other ) const
	{
		return !( ( *this ) > other );
	}
}}
 
