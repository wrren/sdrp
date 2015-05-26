/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Routing/RoutingManager.h>
#include <SDRP/Utilities/Statistics.h>
#include <sstream>

namespace Radicle { namespace SDRP
{
	const RDSize 		RoutingManager::PacketSize 			= 1024;

	const RDUInt8		RoutingManager::DefaultTTL 			= 20;
	
	const RDUInt32		RoutingManager::SequenceNumberDriftTolerance 	= 150;

	const RDTimeStamp	RoutingManager::DefaultMaxRelay 		= 10;

	RoutingManager::RoutingManager( SDRPDelegate& delegate, Node& localNode, RDUInt8 ttl ) :
	m_delegate( delegate ),
	m_node( localNode ),
	m_sequence( 0 ),
	m_ttl( ttl ),
	m_lastRelay( 0 ),
	m_monitor( localNode ),
	m_maxRelay( DefaultMaxRelay )
	{
		m_monitor.Subscribe( this );
	}
	
	void RoutingManager::Purge( const RDTimeStamp maxAge )
	{
		m_monitor.Purge();
		m_routes.Purge( maxAge );
		m_connections.Purge( maxAge );
	}
	
	void RoutingManager::ExpectedNeighbourCount( const RDSize count )
	{
		m_node.Neighbours( m_monitor.NeighbourFilter( true, count ) );
	}
	
	void RoutingManager::Mode( MPRFactory::MPRSelectionMode mode )
	{
		m_monitor.Mode( mode );
	}

	void RoutingManager::MaxRelay( const RDTimeStamp max )
	{
		m_maxRelay = max;
	}
	
	void RoutingManager::HandlePacket( 	const RDNetworkAddress source, 
						const RDUByte8* packet, 
						const RDSize packetSize )
	{
		RDUByte8 type;
		RDSize offset = 0;
		
		if( Serializer::Deserialize( packet, packetSize, offset, offset, type ) )
		{
			if( type == Beacon::Type )
			{
				Beacon beacon;
				
				RD_NLOG( "Received Beacon from Node " << source );
				
				if( beacon.Deserialize( packet, packetSize, 0, offset ) )
				{
					HandleBeacon( source, beacon );
				}
				else
				{
					RD_ERROR( RD_SDRP_ERROR_DESERIALIZATION_FAILURE, "Beacon Deserializate Failed" );
				}
			}
			else if( type == ServiceAdvertisement::Type )
			{
				ServiceAdvertisement advertisement;
								
				if( advertisement.Deserialize( packet, packetSize, 0, offset ) )
				{ 
					HandleAdvertisement( source, advertisement );
				}
				else
				{
					RD_ERROR( RD_SDRP_ERROR_DESERIALIZATION_FAILURE, "Advertisement Deserialization Failed" );
				}
			}
			else
			{
				RD_ERROR( RD_SDRP_ERROR_DESERIALIZATION_FAILURE, "Unrecognized Packet Type" );
			}
		}
		else
		{
			RD_ERROR( RD_SDRP_ERROR_DESERIALIZATION_FAILURE, "Failed to Deserialize Packet" );
		}
	}

	void RoutingManager::SendBeacon()
	{
		if( m_monitor.Mode() == MPRFactory::MPR )
		{
			RDUByte8 buffer[PacketSize];
		
			RD_ASSERT(	m_node.Neighbours().TableSize() > 0,
					RD_SDRP_ERROR_FILTER_SIZE,
					"Node Neighbour Filter has Size 0" );
		
			Beacon beacon( m_node.Address(), m_node.Neighbours() );
			RDSize offset;
		
			if( beacon.Serialize( buffer, PacketSize, 0, offset ) )
			{
				m_delegate.Send( buffer, offset, RD_SDRP_BROADCAST_ADDRESS );
			}
			else
			{
				RD_ERROR( RD_SDRP_ERROR_SERIALIZATION_FAILURE, "Beacon Serialization Failed" );
			}
		}		
	}

	void RoutingManager::SendAdvertisement( ServiceAdvertisement& advertisement )
	{
		RDUByte8 buffer[PacketSize];
		
		advertisement.Destinations( m_monitor.MPRFilter() );
		
		if( m_monitor.Mode() == MPRFactory::ReducedMPR )
		{
			advertisement.Neighbours( m_monitor.NeighbourFilter() );
		}
		else
		{
			advertisement.Neighbours( BloomFilter::Empty );
		}

		RDSize offset;

		if( advertisement.Serialize( buffer, PacketSize, 0, offset ) )
		{
			m_delegate.Send( buffer, offset, RD_SDRP_BROADCAST_ADDRESS );
		}
		else
		{
			RD_ERROR( 	RD_SDRP_ERROR_SERIALIZATION_FAILURE, 
					"Service Advertisement Serialization Failed: " << 
					advertisement.Neighbours().TableSize() << ", " << advertisement.Destinations().TableSize() );
		}
	}
	
	void RoutingManager::SendAdvertisement()
	{
		if( m_node.Services().HasElements() )
		{	
			if( m_sequence + 1 < m_sequence )
			{
				m_sequence = 0;
			}
			else
			{
				m_sequence++;
			}

			ServiceAdvertisement advertisement(	m_node.Address(),
								BloomFilter::Empty,
								m_node.Services(),
								BloomFilter::Empty,
								m_sequence,
								m_ttl );

			SendAdvertisement( advertisement );
		}
	}
	
	void RoutingManager::HandleBeacon(	const RDNetworkAddress source, 
						const Beacon& beacon )
	{
		Node neighbour( source, BloomFilter(), beacon.Neighbours(), m_delegate.Time() );
		
		RD_ASSERT( 	beacon.Neighbours().TableSize() > 0,
				RD_SDRP_ERROR_FILTER_SIZE,
				"Neighbours Filter Deserialized with Size 0" );
		
		m_monitor.NodeWasSeen( neighbour );
	}
	
	void RoutingManager::HandleAdvertisement(	const RDNetworkAddress source, 
							ServiceAdvertisement& advertisement )
	{
		if( m_monitor.Mode() == MPRFactory::ReducedMPR )
		{
			Node neighbour( source, BloomFilter(), advertisement.Neighbours(), m_delegate.Time() );
			m_monitor.NodeWasSeen( neighbour );
		}

		if( advertisement.Source() == m_node.Address() )
		{
			RD_EVENT( "Node " << m_node.Address() << " Received Advertisement from Itself" );
			return;
		}
	
		Route 	newRoute( 	advertisement.Source(),
					source,
					advertisement.Services(),
					advertisement.Hops() );
	
		m_routes.Add( newRoute );
	
		if( m_sequenceRecords.find( advertisement.Source() ) != m_sequenceRecords.end() )
		{
			RDUInt received = advertisement.SequenceNumber(), recorded = m_sequenceRecords[ advertisement.Source() ];		
		
			if( m_sequenceRecords[ advertisement.Source() ] == advertisement.SequenceNumber() ||
			  ( m_sequenceRecords[ advertisement.Source() ] > advertisement.SequenceNumber() &&
			    m_sequenceRecords[ advertisement.Source() ] - advertisement.SequenceNumber() < SequenceNumberDriftTolerance ) )
			{
				return;
			}
		}
		
		m_sequenceRecords[ advertisement.Source() ] = advertisement.SequenceNumber();

		if( advertisement.Hops() > advertisement.MaximumTTL() )
		{
			return;
		}
		// If we're in the destinations filter or not in the neighbour filter, relay the advertisement
		if( 	advertisement.Destinations().Contains( m_node.Address() ) ||
			( 	m_monitor.Mode() == MPRFactory::ReducedMPR && 
				advertisement.Neighbours().Contains( m_node.Address() ) == false ) ) 
		{	
			advertisement.HopsIncrement();
			SendAdvertisement( advertisement );
		}
	}
	
	void RoutingManager::OnNeighbourLost( const Node& neighbour )
	{
		m_node.Neighbours().Remove( neighbour.Address() );
	}
	
	void RoutingManager::OnNeighbourAdded( const Node& neighbour )
	{
		m_node.Neighbours().Insert( neighbour.Address() );
	}
	
	RoutingManager::RouteSet RoutingManager::RoutesToService( const RDServiceIdentifier service ) const
	{
		return m_routes.RoutesToService( service );
	}
	
	bool RoutingManager::HasRoutesToService( const RDServiceIdentifier service ) const
	{
		return m_routes.HasRoutesToService( service );
	}

	RoutingManager::RouteSet RoutingManager::RoutesToHost( const RDNetworkAddress address ) const
	{
		return m_connections.RoutesToHost( address );
	}
	
	bool RoutingManager::HasRoutesToHost( const RDNetworkAddress host ) const
	{
		return m_connections.HasRouteToHost( host );
	}

	void RoutingManager::RegisterConnection( 	const RDNetworkAddress sourceAddress, 
							const RDNetworkAddress lastHop )
	{
		if( m_routes.HasRouteToHost( sourceAddress ) == false )
		{
			Route connection(	sourceAddress,
						lastHop,
						BloomFilter::Empty,
						Route::MaxHops );
					
			m_connections.Add( connection );	
		}
	}
} }

