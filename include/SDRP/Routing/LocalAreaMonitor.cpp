/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#include <SDRP/Routing/LocalAreaMonitor.h>

namespace Radicle { namespace SDRP 
{
	const RDTime LocalAreaMonitor::DefaultMaxAge = 15;

	LocalAreaMonitor::LocalAreaMonitor( const Node& localNode ) : 	m_maxAge( LocalAreaMonitor::DefaultMaxAge ),
									m_cacheIsValid( false ),
									m_mode( MPRFactory::MPR ),
									m_calculatorFactory( localNode )
	{
		m_calculator = m_calculatorFactory.GetCalculator( m_mode );
	}

	const BloomFilter& LocalAreaMonitor::MPRFilter()
	{
		/*
		if( m_cacheIsValid )
		{
			return m_mpr;
		}
		*/

		if( m_neighbours.size() == 0 )
		{
			return m_mpr.DestructiveResize( 1, 0 ).Universe();
		}

		m_mpr = m_calculator->Calculate( m_neighbours );

		m_cacheIsValid = true;

		return m_mpr;
	}

	const BloomFilter& LocalAreaMonitor::NeighbourFilter( bool constrain, const RDSize size )
	{
		return m_neighbourFilter;
	}

	void LocalAreaMonitor::Mode( MPRFactory::MPRSelectionMode mode )
	{
		m_mode = mode;
		m_calculator = m_calculatorFactory.GetCalculator( mode );
		m_cacheIsValid = false;
	}

	const MPRFactory::MPRSelectionMode LocalAreaMonitor::Mode() const
	{
		return m_mode;
	}

	void LocalAreaMonitor::MaxNeighbourAge( const RDTime max )
	{
		m_maxAge = max;
	}

	RDTime LocalAreaMonitor::MaxNeighbourAge() const
	{
		return m_maxAge;
	}

	void LocalAreaMonitor::RebuildNeighbourFilter()
	{

		if( m_neighbours.size() == 0 )
		{
			m_neighbourFilter = BloomFilter::Empty;
			return;
		}

		m_neighbourFilter.ResetWithParameters( m_neighbours.size() );

		for( NodeContainer::iterator i = m_neighbours.begin(); i != m_neighbours.end(); i++ )
		{
			m_neighbourFilter.Insert( i->Address() );
		}
	}

	void LocalAreaMonitor::NodeWasSeen( const Node& node )
	{
		std::pair<NodeContainer::iterator, bool> result = m_neighbours.insert( node );

		if( result.second == true )
		{
			for( SubscriberContainer::iterator i = Subscribers().begin(); i != Subscribers().end(); i++ )
			{
				m_neighbourFilter.Insert( node.Address() );
				( *i )->OnNeighbourAdded( node );
			}
		}

		try
		{
			if( result.second == true || result.first->Neighbours() != node.Neighbours() )
			{
				m_cacheIsValid = false;
				RebuildNeighbourFilter();
			}
		}
		catch( BloomFilterSizeMismatchException e )
		{
			m_cacheIsValid = false;
		}
		

		m_neighbours.erase( result.first );
		result = m_neighbours.insert( node );
	}

	void LocalAreaMonitor::Purge()
	{
		RDTimeStamp now = Logger::Time();

		for( NodeContainer::iterator i = m_neighbours.begin(); i != m_neighbours.end();  )
		{
			if( now - i->LastSeen() > m_maxAge )
			{
				for( SubscriberContainer::iterator j = Subscribers().begin(); j != Subscribers().end(); j++ )
				{
					( *j )->OnNeighbourLost( ( *i ) );
				}

				m_neighbours.erase( i++ );
				m_cacheIsValid = false;
			}
			else
			{
				i++;
			}
		}

		RebuildNeighbourFilter();
	}
}}
