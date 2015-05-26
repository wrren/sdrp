/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_SERVICE_ADVERTISEMENT_H
#define RD_SDRP_SERVICE_ADVERTISEMENT_H

#include <SDRP/Core/Core.h>

namespace Radicle { namespace SDRP
{
	class ServiceAdvertisement : public ISerializable
	{
	public:
	
		/// Service Advertisement Packet Type
		static const RDUByte8	Type;
	
		/**
		 *	Default Constructor
		 */
		ServiceAdvertisement();
		
		/**
		 *	Initializing Constructor
		 * @param source	Source Node Address
		 * @param destinations	Bloom Filter Describing Receiving Nodes
		 * @param services 	Bloom Filter Describing Advertised Services
		 * @param neighbours 	Neighbour Nodes
		 * @param sequence	Packet Sequence Number
		 * @param maxTTL	Maximum TTL
		 */
		ServiceAdvertisement(	const RDNetworkAddress source,
					const BloomFilter& destinations,
					const BloomFilter& services,
					const BloomFilter& neighbours,
					const RDUInt8 sequence,
					const RDUInt8 maxTTL );
	
		/**
		 *	Get the source network address
		 * @return	source network address
		 */
		const RDNetworkAddress Source() const;
		
		/**
		 *	Set the source network address
		 * @param	source	Source Network Address 
		 */
		void Source( const RDNetworkAddress source );
		
		/**
		 *	Get the destination nodes for this advertisement
		 * @return	destination nodes for this advertisement
		 */
		const BloomFilter& Destinations() const;
		
		/**
		 *	Set the destination nodes for this advertisement
		 * @param	destinations destination nodes for this advertisement 
		 */
		void Destinations( const BloomFilter& destinations );

		/**
		 *	Get the neighbour node bloom filter for this advertisement
		 * @return 	Neighbour Node Bloom Filter
		 */
		const BloomFilter& Neighbours() const;

		/**
		 *	Set the neighbour node bloom filter for this advertisement
		 * @param neighbours 	Neighbour Node Bloom Filter
		 */
		void Neighbours( const BloomFilter& neighbours );
		
		/**
		 *	Get the services advertiseed by this packet
		 * @return	services advertiseed by this packet
		 */
		const BloomFilter& Services() const;
		
		/**
		 *	Set the services advertiseed by this packet
		 * @param	services	services advertiseed by this packet 
		 */
		void Services( const BloomFilter& services );
		
		/**
		 *	Get the packet sequence number
		 * @return	packet sequence number
		 */
		const RDUInt8 SequenceNumber() const;
		
		/**
		 *	Set the packet sequence number
		 * @param	sequence	packet sequence number 
		 */
		void SequenceNumber( const RDUInt8 sequence );
		
		/**
		 *	Get the number of hops traversed by this packet
		 * @return	number of hops traversed by this packet
		 */
		const RDUInt8 Hops() const;
		
		/**
		 *	Set the number of hops traversed by this packet
		 * @param	hops	number of hops traversed by this packet 
		 */
		void Hops( const RDUInt8 hops );
		
		/**
		 *	Increment the number of hops traversed by this packet by one
		 */
		void HopsIncrement();
		
		/**
		 *	Get the maximum recommended TTL
		 * @return	maximum recommended TTL
		 */
		const RDUInt8 MaximumTTL() const;
		
		/**
		 *	Set the maximum recommended TTL
		 * @param	maxTTL	maximum recommended TTL 
		 */
		void MaximumTTL( const RDUInt8 maxTTL );
		
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
	
		/// Source Network Address
		RDNetworkAddress	m_source;
		/// Destination Nodes
		BloomFilter		m_destinations;
		/// Neighbour Nodes
		BloomFilter 		m_neighbours;
		/// Advertised Services
		BloomFilter		m_services;
		/// Packet Sequence Number
		RDUInt32		m_sequence;
		/// Number of hops traversed by packet
		RDUInt8			m_hops;	
		/// Maximum Recommended TTL
		RDUInt8			m_maxTTL;
	};
} }

#endif // RD_SDRP_SERVICE_ADVERTISEMENT_H

