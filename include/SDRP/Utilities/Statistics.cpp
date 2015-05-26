/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Utilities/Statistics.h>

namespace Radicle { namespace SDRP
{
	Statistics* Statistics::m_instance = NULL;

	Statistics* Statistics::Instance()
	{
		if( m_instance == NULL )
		{
			m_instance = new Statistics();
		}
		
		return m_instance;
	}

	Statistics::Statistics() :
	m_firstPacket( 0 ),
	m_lastPacket( 0 ),
	m_packetBytes( 0 ),
	m_packetCount( 0 )
	{}
	
	void Statistics::PacketSent( const RDSize packetSize )
	{
		if( m_firstPacket == 0.0 )
		{
			m_firstPacket = Logger::Time();
		}
		
		m_lastPacket = Logger::Time();
		m_packetBytes += packetSize;
		m_packetCount++;
	}
	
	const RDSize Statistics::PacketCount() const
	{
		return m_packetCount;
	}

	const RDSize Statistics::PacketBytes() const
	{
		return m_packetBytes;
	}
	
	const RDSize Statistics::Overhead() const
	{
		if( m_firstPacket != 0.0 )
		{
			return m_packetBytes / ( m_lastPacket - m_firstPacket );
		}
		
		return 0;
	}
	
	void Statistics::ReportRaw( std::ostream& out ) const
	{
		
	}
} }

