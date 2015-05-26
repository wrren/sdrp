/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_ROUTING_MANAGER_H
#define RD_SDRP_ROUTING_MANAGER_H

#include <SDRP/SDRPDelegate.h>
#include <SDRP/Core/Core.h>
#include <SDRP/Routing/Node.h>
#include <SDRP/Routing/LocalAreaMonitor.h>
#include <SDRP/Routing/RouteTable.h>
#include <SDRP/Packets/Beacon.h>
#include <SDRP/Packets/ServiceAdvertisement.h>

namespace Radicle { namespace SDRP
{
	/**
	 *	The Routing Manager manages routes to services, packet formation
	 *	and packet interpretation
	 */
	class RoutingManager : public LocalAreaListener
	{
	public:

		
	
		/// Convenience Typedef
		typedef std::set<Route>	RouteSet;
	
		/// Default Service Advertisement TTL
		static const RDUInt8 		DefaultTTL;
		/// Drift Tolerance for Sequence Number Acknowledgement
		static const RDUInt32 		SequenceNumberDriftTolerance;
		/// Default Max Relay Time
		static const RDTimeStamp	DefaultMaxRelay;
	
		/**
		 *	Default Constructor
		 * @param delegate	Delegate Used to Send Packets
		 * @param localNode	Local Node
		 * @param ttl		TTL for Service Advertisements
		 */
		RoutingManager( SDRPDelegate& delegate, Node& localNode, RDUInt8 ttl = DefaultTTL );

		/**
		 *	Handle a received SDRP packet
		 * @param source	Source Node Address
		 * @param packet	Packet Data Buffer
		 * @param packetSize	Packet Size in Bytes
		 */
		void HandlePacket( 	const RDNetworkAddress source, 
					const RDUByte8* packet, 
					const RDSize packetSize );
		
		/**
		 *	Have the Routing Manager form and send a beacon packet
		 */
		void SendBeacon();
		
		/**
		 *	Have the Routing Manager form and send a service advertisement packet
		 */
		void SendAdvertisement();
		
		/**
		 *	Purge stale routes and neighbours
		 */
		void Purge( const RDTimeStamp maxAge );
		
		/**
		 *	Set the expected neighbour count. This will modify the size of the beacon packet
		 *	bloom filter used
		 * @param count		Expected number of neighbours
		 */
		void ExpectedNeighbourCount( const RDSize count );
		
		/**
		 *	Set the advertisement mode for the routing manager
		 * @param mode 	Advertisement Mode
		 */
		void Mode( MPRFactory::MPRSelectionMode mode );
		
		/**
		 *	Get all routes to the specified service
		 * @param service	Requested Service
		 * @return		Routes to Requested Service
		 */
		RouteSet RoutesToService( const RDServiceIdentifier service ) const;

		/**
		 *	Check whether the routing manager has any routes to the specified service
		 * @return 	True - If a route to the specified service exists. False otherwise.
		 */
		bool HasRoutesToService( const RDServiceIdentifier service ) const;
		
		/**
		 *	Get all routes to the specified address
		 * @param address	Target network address
		 * @return		Routes to requested address
		 */
		RouteSet RoutesToHost( const RDNetworkAddress address ) const;

		/**
		 *	Check whether the routing manager has any routes to the specified host
		 * @return 	True - If a route to the specified host exists. False otherwise.
		 */
		bool HasRoutesToHost( const RDNetworkAddress host ) const;

		/**
		 *	Maximum time between service advertisement relays
		 * @param max 	Max Relay Time
		 */
		void MaxRelay( const RDTimeStamp max );
		
		/**
		 *	LocalAreaListener Callback. The specified node was purged due to
		 *	inactivity
		 * @param neighbour	Purged Neighbour Node
		 */
		virtual void OnNeighbourLost( const Node& neighbour );
		
		/**
		 *	LocalAreaListener Callback. The specified node was added to the neighbour list
		 * @param neighbour	Added Neighbour Node
		 */
		virtual void OnNeighbourAdded( const Node& neighbour );
	
		/**
		 *	Register a connection. Used to maintain a route back to a client.
		 * @param sourceAddress	Source Network Address
		 * @param lastHop	Last Hop Address
		 * @param sourceService	Source Service Identifier
		 */
		void RegisterConnection( 	const RDNetworkAddress sourceAddress, 
						const RDNetworkAddress lastHop );
	
	private:
	
		/// Size of Packet Buffer
		static const RDSize 	PacketSize;
		
		/**
		 *	Calculate MPRs and return address bloom filter
		 * @return	MPR Bloom Filter
		 */
		BloomFilter CalculateMPRFilter() const;
		
		/**
		 *	Handle a Beacon Packet
		 */
		void HandleBeacon(	const RDNetworkAddress source, 
					const Beacon& beacon );
		
		/**
		 *	Handle a Service Advertisement Packet
		 */
		void HandleAdvertisement(	const RDNetworkAddress source, 
						ServiceAdvertisement& advertisement );

		/**
		 *	Send an advertisement
		 */
		void SendAdvertisement( ServiceAdvertisement& advertisement );
	
		/// Delegate used for Sending Packets
		SDRPDelegate&				m_delegate;
		/// Local Node
		Node&					m_node;
		/// Incrementing Sequence Number
		RDUInt8					m_sequence;
		/// Service Advertisement TTL
		RDUInt8					m_ttl;
		/// Route Table
		RouteTable				m_routes;
		/// Routes back to clients
		RouteTable				m_connections;
		/// Sequence Number Records
		std::map<RDNetworkAddress, RDUInt8>	m_sequenceRecords;
		/// Time Since Last Relay
		RDTimeStamp 				m_lastRelay;
		/// Max Time Until Next Relay
		RDTimeStamp				m_maxRelay;
		/// Local Area Monitor / MPR Calculator
		LocalAreaMonitor			m_monitor;
	};
} }

#endif // RD_SDRP_ROUTING_MANAGER_H
	
