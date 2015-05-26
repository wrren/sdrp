/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Packets/Beacon.h>
#include <SDRP/Utilities/Serializer.h>

namespace Radicle { namespace SDRP
{
	const RDUByte8 Beacon::Type = 0x00;

	Beacon::Beacon() :
	m_source( RD_SDRP_UNSPECIFIED_ADDRESS )
	{}
	
	Beacon::Beacon(	const RDNetworkAddress source,
			const BloomFilter& neighbours ):
	m_source( source ),
	m_neighbours( neighbours )
	{}
	
	const BloomFilter& Beacon::Neighbours() const
	{
		return m_neighbours;
	}

	void Beacon::Neighbours( const BloomFilter& var )
	{
		m_neighbours = var;
	}
	
	const RDNetworkAddress Beacon::Source() const
	{
		return m_source;
	}
	
	void Beacon::Source( const RDNetworkAddress var )
	{
		m_source = var;
	}
	
	bool Beacon::Serialize( RDUByte8* buffer,
				const RDSize bufferSize,
				const RDSize offset,
				RDSize& newOffset ) const
	{
		return	Serializer::Serialize( buffer, bufferSize, offset, newOffset, Beacon::Type ) &&
			Serializer::Serialize( buffer, bufferSize, newOffset, newOffset, m_source ) &&
			m_neighbours.Serialize( buffer, bufferSize, newOffset, newOffset );
	}

	bool Beacon::Deserialize(	const RDUByte8* buffer,
 					const RDSize bufferSize,
					const RDSize offset,
					RDSize& newOffset )
	{
		RDUByte8 packetType;
		
		if( Serializer::Deserialize( buffer, bufferSize, offset, newOffset, packetType ) )
		{
			if( packetType == Beacon::Type )
			{
				return	Serializer::Deserialize( buffer, bufferSize, newOffset, newOffset, m_source ) &&
					m_neighbours.Deserialize( buffer, bufferSize, newOffset, newOffset );
			}
			else
			{
				RD_ERROR( RD_SDRP_ERROR_PACKET_TYPE, "Beacon Deserialized Incorrect Packet Type" );
			}
		}
		else
		{
			RD_ERROR( RD_SDRP_ERROR_DESERIALIZATION_FAILURE, "Failed to Deserialize Beacon" );
		}
	
		return false;
	}
} }
 
