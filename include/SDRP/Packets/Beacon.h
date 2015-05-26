/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_BEACON_H
#define RD_SDRP_BEACON_H

#include <SDRP/Core/Core.h>

namespace Radicle { namespace SDRP
{
	/**
	 *	Beacon packet used to advertise neighbour identities
	 */
	class Beacon: public ISerializable
	{
	public:
	
		/// Beacon Packet Type Identifier
		static const RDUByte8 Type;
	
		/**
		 *	Default Constructor
		 */
		Beacon();
		
		/**
		 *	Initializing Constructor
		 * @param source	Source Node Address
		 * @param neighbours	Bloom filter describing node neighbour identities
		 */
		Beacon( const RDNetworkAddress source,
			const BloomFilter& neighbours );
			
		
		/**
		 *	Get this beacon's source node address
		 * @return	Source Node Address
		 */
		const RDNetworkAddress Source() const;
		
		/**
		 *	Set this beacon's source node address
		 * @param	Source Node Address
		 */
		void Source( const RDNetworkAddress source );
		
		/**
		 *	Get the neighbour bloom filter
		 * @return	neighbour bloom filter
		 */
		const BloomFilter& Neighbours() const;
		
		/**
		 *	Set the neighbour bloom filter
		 * @param	neighbours  Neighbour Bloom Filter
		 */
		void Neighbours( const BloomFilter& neighbours );
		
		/**
		 *	Serialize this object into the provided data buffer.
		 * @param buffer	Data buffer into which the object should be serialized
		 * @param bufferSize	Size of the data buffer in bytes
		 * @param offset	Offset into the buffer at which serialization should begin
		 * @param newOffset	New offset produced by serialization
		 * @return		True - If serialization was successful. False otherwise.
		 */
		virtual bool Serialize( 	RDUByte8* buffer,
						const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset ) const;
			
		/**
		 *	Deserialize the this object from the provided data buffer
		 * @param buffer	Data buffer from which the object should be deserialized
		 * @param bufferSize	Size of the buffer in bytes
		 * @param offset	Offset into the buffer at which deserialization should begin
		 * @param newOffset	New offset produced by deserializing the object
		 * @return		True - If deserialization was successful. False otherwise.
		 */
		virtual bool Deserialize( 	const RDUByte8* buffer,
		 				const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset );
	
	private:
	
		/// Local Node Address
		RDNetworkAddress	m_source;
		/// Neighbour List
		BloomFilter		m_neighbours;
	};
} }

#endif // RD_SDRP_BEACON_H
 
