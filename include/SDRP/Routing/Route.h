/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_ROUTE_H
#define RD_SDRP_ROUTE_H

#include <SDRP/Core/Core.h>

namespace Radicle { namespace SDRP
{
	/**
	 *	Class Representing a Route to a Particular Service
	 */
	class Route
	{
	public:
	
		/// Maximum Number of Hops
		static const RDUInt8 MaxHops;
		
		/**
		 *	Default Constructor
		 */
		Route();
		
		/**
		 *	Initializing Constructor
		 * @param server	Server Address
		 * @param nextHop	Next Hop to Server
		 * @param services	Services Advertised by Server
		 * @param hops		Number of Hops to Server
		 */
		Route(	const RDNetworkAddress server,
			const RDNetworkAddress nextHop,
			const BloomFilter& services,
			const RDUInt8 hops );
			
		/**
		 *	Get the server address
		 * @return	server address
		 */
		const RDNetworkAddress Server() const;
		
		/**
		 *	Set the server address
		 * @param	server	server address 
		 */
		void Server( const RDNetworkAddress server );
		
		/**
		 *	Get the next hop addresse
		 * @return	next hop addresse
		 */
		const RDNetworkAddress NextHop() const;
		
		/**
		 *	Set the next hop addresse
		 * @param	nextHop	next hop addresse 
		 */
		void NextHop( const RDNetworkAddress nextHop );
		
		/**
		 *	Get the service endpoint for this route
		 * @return	service endpoint for this route
		 */
		const BloomFilter& Services() const;
		
		/**
		 *	Set the service endpoint for this route
		 * @param	services	service endpoint for this route 
		 */
		void Services( const BloomFilter& services );
		
		/**
		 *	Get the age of this route
		 * @return	age of this route
		 */
		const RDTimeStamp Age() const;
		
		/**
		 *	Set the age of this route
		 * @param	age	age of this route 
		 */
		void Age( const RDTimeStamp age );
		
		/**
		 *	Get the number of hops for this route
		 * @return	number of hops for this route
		 */
		const RDUInt8 Hops() const;
		
		/**
		 *	Set the number of hops for this route
		 * @param	hops	number of hops for this route 
		 */
		void Hops( const RDUInt8 hops );
		
		/**
		 *	Comparison Operators
		 */
		bool operator==( const Route& other ) const;
		bool operator<( const Route& other ) const;
		bool operator>( const Route& other ) const;
		
	private:
	
		/// Server Endpoint
		RDNetworkAddress	m_server;
		/// Next Hop on Route
		RDNetworkAddress	m_nextHop;
		/// Route Services
		BloomFilter		m_services;
		/// Route Age
		RDTimeStamp 		m_age;
		/// Number of Hops to Service
		RDUInt8			m_hops;
	};
} }

#endif // RD_SDRP_ROUTE_H
