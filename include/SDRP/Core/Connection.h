/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_CONNECTION_H
#define RD_SDRP_CONNECTION_H

#include <SDRP/Core/Types.h>

namespace Radicle { namespace SDRP
{
	class Connection
	{
	public:
		
		/// Connection Type
		enum Type
		{
			Client,
			Server
		};
		
		/**
		 *	Default Constructor
		 */
		Connection();
	
		/**
		 *	Initializing Constructor
		 * @param address	Endpoint Address
		 * @param nextHop	Next Hop to Address
		 * @param service	Endpoint Service
		 * @param time		Connection Creation/Update Time
		 */
		Connection( 	const RDNetworkAddress address, 
				const RDNetworkAddress nextHop,
				const RDServiceIdentifier service,
				const RDTimeStamp time );
		
		/**
		 *	Get/Set Connection Age
		 */
		const RDTimeStamp Age() const;
		void Age( const RDTimeStamp age );
		
		/**
		 *	Get/Set Endpoint Address
		 */
		const RDNetworkAddress Address() const;
		void Address( const RDNetworkAddress address );

		/**
		 *	Get/Set Next Hop Address
		 */
		const RDNetworkAddress NextHop() const;
		void NextHop( const RDNetworkAddress nextHop );
		
		/**
		 *	Get/Set Endpoint Service
		 */
		const RDServiceIdentifier Service() const;
		void Service( const RDServiceIdentifier service );
		
		/**
		 *	Comparison Operators
		 */
		bool operator==( const Connection& other ) const;
		bool operator<( const Connection& other ) const;
		bool operator>( const Connection& other ) const;
	
	private:
	
		/// Network Address
		RDNetworkAddress	m_address;
		/// Next Hop
		RDNetworkAddress	m_nextHop;
		/// Endpoint Service
		RDServiceIdentifier	m_service;
		/// Last Update
		RDTimeStamp		m_time;
		/// Connection Type
		Type			m_type;
	};
}}

#endif // RD_SDRP_CONNECTION_H
 
