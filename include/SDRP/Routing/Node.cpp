/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/

#include <SDRP/Routing/Node.h>

namespace Radicle { namespace SDRP
{
	Node::Node() :
	m_address( RD_SDRP_UNSPECIFIED_ADDRESS ),
	m_services( 5, 0.1 )
	{}
	
	Node::Node( const Node& other ) :
	m_address( other.m_address ),
	m_services( other.m_services ),
	m_neighbours( other.m_neighbours ),
	m_lastSeen( other.m_lastSeen )
	{}

	Node::Node(	const RDNetworkAddress address,
			const BloomFilter& services,
			const BloomFilter& neighbours,
			const RDTimeStamp lastSeen ) :
	m_address( address ),
	m_services( services ),
	m_neighbours( neighbours ),
	m_lastSeen( lastSeen )
	{}
	
	const RDNetworkAddress Node::Address() const
	{
		return m_address;
	}
	
	void Node::Address( const RDNetworkAddress address )
	{
		m_address = address;
	}

	const BloomFilter& Node::Services() const
	{
		return m_services;
	}

	BloomFilter& Node::Services()
	{
		return m_services;
	}

	void Node::Services( const BloomFilter& services )
	{
		m_services = services;
	}

	const BloomFilter& Node::Neighbours() const
	{
		return m_neighbours;
	}
	
	BloomFilter& Node::Neighbours()
	{
		return m_neighbours;
	}

	void Node::Neighbours( const BloomFilter& neighbours )
	{
		m_neighbours = neighbours;
	}

	RDTimeStamp Node::LastSeen() const
	{
		return m_lastSeen;
	}

	void Node::LastSeen( const RDTimeStamp lastSeen )
	{
		m_lastSeen = lastSeen;
	}
	
	bool Node::operator==( const Node& other ) const
	{
		return other.m_address == m_address;
	}
	
	bool Node::operator>( const Node& other ) const
	{
		return m_address == other.m_address ? false : m_address > other.m_address;
	}
	
	bool Node::operator<( const Node& other ) const
	{
		return m_address == other.m_address ? false : m_address < other.m_address;
	}
} }

