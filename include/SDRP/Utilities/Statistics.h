/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_STATISTICS_H
#define RD_SDRP_STATISTICS_H

#include <SDRP/Core/Core.h>
#include <ostream>

namespace Radicle { namespace SDRP
{
	class Statistics
	{
	public:
	
		/**
		 *	Get a Statistics Singleton
		 * @return	Statistics Object or NULL on Failure
		 */
		static Statistics* Instance();
		
		/**
		 *	Register a packet send event
		 * @param packetSize	Packet Size
		 */
		void PacketSent( const RDSize packetSize );
		
		/**
		 *	Get the total number of packets sent
		 */
		const RDSize PacketCount() const;
		
		/**
		 *	Get the total number of bytes sent in packet data
		 */
		const RDSize PacketBytes() const;
		
		/**
		 *	Get the packet overhead in bytes/sec
		 */	
		const RDSize Overhead() const;
		
		/**
		 *	Report Packet Statistics in raw form to the provided output stream
		 */
		void ReportRaw( std::ostream& out ) const;
	
	private:

		/// Singleton Instance
		static Statistics* m_instance;
		
		/// Time at which first packet was sent
		RDTimeStamp	m_firstPacket;
		/// Time at which last packet was sent
		RDTimeStamp	m_lastPacket;
		/// Bytes of packet data sent
		RDSize		m_packetBytes;
		/// Number of packets sent
		RDSize		m_packetCount;

		/**
		 *	Default Constructor
		 */
		Statistics();	
	};
} }

#endif // RD_SDRP_STATISTICS_H

