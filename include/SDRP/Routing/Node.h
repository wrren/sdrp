/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_NODE_H
#define RD_SDRP_NODE_H

#include <SDRP/Core/Core.h>

namespace Radicle { namespace SDRP
{
	/**
	 *	Represents a single node in the network
	 */
	class Node
	{
	public:
		
		/**
		 *	Default Constructor
		 */
		Node();
		
		/**
		 *	Assignment Constructor. Assign this node the data of other.
		 * @param other 	Node from which to assign data
		 */
		Node( const Node& other );

		/**
		 *	Initializing Constructor
		 * @param address	Node Network Address
		 * @param services	Node Services
		 * @param neighbours	Node Neighbours
		 * @param lastSeen 	Node Last Seen Time
		 */
		Node(	const RDNetworkAddress address,
			const BloomFilter& services,
			const BloomFilter& neighbours,
			const RDTimeStamp lastSeen );
		
		/**
		 *	Get this node's network address
		 * @return	Node Network Address
		 */
		const RDNetworkAddress Address() const;
		
		/**
		 *	Set this node's network address
		 * @param address	New Network Address
		 */
		void Address( const RDNetworkAddress address );
		
		/**
		 *	Get a bloom filter describing the services provided by this node
		 * @return	Services Bloom Filter
		 */
		const BloomFilter& Services() const;
		
		/**
		 *	Get a bloom filter describing the services provided by this node
		 * @return	Services Bloom Filter
		 */
		BloomFilter& Services();
		
		/**
		 *	Set the bloom filter describing the services provided by this node
		 * @param	service		Services Bloom Filter
		 */
		void Services( const BloomFilter& services );
		
		/**
		 *	Get a bloom filter describing the neighbours of this node
		 * @return	Neighbours Bloom Filter
		 */
		const BloomFilter& Neighbours() const;
		
		/**
		 *	Get a bloom filter describing the neighbours of this node
		 * @return	Neighbours Bloom Filter
		 */
		BloomFilter& Neighbours();
		
		/**
		 *	Set the bloom filter describing the neighbours of this node
		 * @param	neighbours	Neighbours Bloom Filter
		 */
		void Neighbours( const BloomFilter& neighbours );

		/**
		 *	Get or Set the Node Last Seen Time
		 */
		RDTimeStamp LastSeen() const;
		void LastSeen( const RDTimeStamp lastSeen );
		
		/**
		 *	Comparison Operators
		 */
		bool operator==( const Node& other ) const;
		bool operator<( const Node& other ) const;
		bool operator>( const Node& other ) const;
		
	private:
		
		/// Node Network Address
		RDNetworkAddress	m_address;
		/// Node Last Seen Time
		RDTimeStamp 		m_lastSeen;
		/// Bloom Filter containing this node's services
		BloomFilter		m_services;
		/// Bloom Filter containing this node's neighbours
		BloomFilter		m_neighbours;
	};
} }

#endif // RD_SDRP_NODE_H
 
