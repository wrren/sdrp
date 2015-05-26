/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Packets/ServiceAdvertisement.h>

namespace Radicle { namespace SDRP
{
	const RDUByte8 ServiceAdvertisement::Type = 0x01;
	
	ServiceAdvertisement::ServiceAdvertisement() :
	m_source( RD_SDRP_UNSPECIFIED_ADDRESS ),
	m_maxTTL( 0 ),
	m_sequence( 0 ),
	m_hops( 0 )
	{}
	
	ServiceAdvertisement::ServiceAdvertisement(	const RDNetworkAddress source,
							const BloomFilter& destinations,
							const BloomFilter& services,
							const BloomFilter& neighbours,
							const RDUInt8 sequence,
							const RDUInt8 maxTTL ) :
	m_source( source ),
	m_destinations( destinations ),
	m_services( services ),
	m_neighbours( neighbours ),
	m_sequence( sequence ),
	m_hops( 0 ),
	m_maxTTL( maxTTL )
	{}
	
	const RDNetworkAddress ServiceAdvertisement::Source() const
	{
		return m_source;
	}
	
	void ServiceAdvertisement::Source( const RDNetworkAddress var )
	{
		m_source = var;
	}
	
	const BloomFilter& ServiceAdvertisement::Destinations() const
	{
		return m_destinations;
	}
	
	void ServiceAdvertisement::Destinations( const BloomFilter& var )
	{
		m_destinations = var;
	}
	
	const BloomFilter& ServiceAdvertisement::Services() const
	{
		return m_services;
	}
	
	void ServiceAdvertisement::Services( const BloomFilter& var )
	{
		m_services = var;
	}

	const BloomFilter& ServiceAdvertisement::Neighbours() const
	{
		return m_neighbours;
	}
	
	void ServiceAdvertisement::Neighbours( const BloomFilter& neighbours )
	{
		m_neighbours = neighbours;
	}
	
	const RDUInt8 ServiceAdvertisement::SequenceNumber() const
	{
		return m_sequence;
	}
	
	void ServiceAdvertisement::SequenceNumber( const RDUInt8 var )
	{
		m_sequence = var;
	}
	
	const RDUInt8 ServiceAdvertisement::Hops() const
	{
		return m_hops;
	}
	
	void ServiceAdvertisement::Hops( const RDUInt8 var )
	{
		m_hops = var;
	}
	
	void ServiceAdvertisement::HopsIncrement()
	{
		m_hops++;
	}
	
	const RDUInt8 ServiceAdvertisement::MaximumTTL() const
	{
		return m_maxTTL;
	}
	
	void ServiceAdvertisement::MaximumTTL( const RDUInt8 var )
	{
		m_maxTTL = var;
	}
	
	bool ServiceAdvertisement::Serialize( 	RDUByte8* buffer,
						const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset ) const
	{
		return	Serializer::Serialize( 		buffer, bufferSize, offset, newOffset, ServiceAdvertisement::Type ) &&
			Serializer::Serialize( 		buffer, bufferSize, newOffset, newOffset, m_source ) &&
			m_destinations.Serialize( 	buffer, bufferSize, newOffset, newOffset ) &&
			m_neighbours.Serialize( 	buffer, bufferSize, newOffset, newOffset ) &&
			m_services.Serialize( 		buffer, bufferSize, newOffset, newOffset ) &&
			Serializer::Serialize( 		buffer, bufferSize, newOffset, newOffset, m_sequence ) &&
			Serializer::Serialize( 		buffer, bufferSize, newOffset, newOffset, m_hops ) &&
			Serializer::Serialize(		buffer, bufferSize, newOffset, newOffset, m_maxTTL );
	}

	bool ServiceAdvertisement::Deserialize(	const RDUByte8* buffer,
 						const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset )
	{
		RDUByte8 packetType;
		
		if( Serializer::Deserialize( buffer, bufferSize, offset, newOffset, packetType ) )
		{
			if( packetType == ServiceAdvertisement::Type )
			{
				return	Serializer::Deserialize( 	buffer, bufferSize, newOffset, newOffset, m_source ) &&
					m_destinations.Deserialize( 	buffer, bufferSize, newOffset, newOffset ) &&
					m_neighbours.Deserialize( 	buffer, bufferSize, newOffset, newOffset ) &&
					m_services.Deserialize( 	buffer, bufferSize, newOffset, newOffset ) &&
					Serializer::Deserialize( 	buffer, bufferSize, newOffset, newOffset, m_sequence ) &&
					Serializer::Deserialize( 	buffer, bufferSize, newOffset, newOffset, m_hops ) &&
					Serializer::Deserialize(	buffer, bufferSize, newOffset, newOffset, m_maxTTL ); 
			}
			else
			{
				RD_ERROR( RD_SDRP_ERROR_PACKET_TYPE, "Service Advertisement Deserialized Incorrect Packet Type" );
			}
		}
	
		return false;
	}
} }

