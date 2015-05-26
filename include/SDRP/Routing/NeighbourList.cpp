/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Routing/NeighbourList.h>

namespace Radicle { namespace SDRP
{
	NeighbourList::Iterator::Iterator( std::vector< std::pair<RDTimeStamp, Node> >::const_iterator i ) :
	m_i( i )
	{}

	NeighbourList::NeighbourList( NeighbourListDelegate& delegate, Node& localNode ) :
	m_delegate( delegate ),
	m_localNode( localNode )
	{}
	
	void NeighbourList::Add( const Node& neighbour )
	{
		for( 	std::vector< std::pair< RDTimeStamp, Node> >::iterator i = m_neighbours.begin();
			i != m_neighbours.end(); ++i )
		{
			if( i->second == neighbour )
			{
				i->second = neighbour;
				i->first = Logger::Time();
				return;
			}
		}
		
		std::pair< RDTimeStamp, Node > p( Logger::Time(), neighbour );
		m_neighbours.push_back( p );
		
		RD_NLOG( "Adding New Neighbour: Node " << neighbour.Address() );
		
		m_delegate.OnNeighbourAdded( *this, neighbour );
	}
	
	void NeighbourList::Purge( const RDTimeStamp maxAge )
	{
		RDUInt removed = 0;
		RDTimeStamp time = Logger::Time();
	
		for( 	std::vector< std::pair< RDTimeStamp, Node> >::iterator i = m_neighbours.begin();
			i != m_neighbours.end(); ++i )
		{
			if( ( time - i->first ) > maxAge )
			{
				RD_NLOG( "Purging Neighbour: Node " << i->second.Address() );
			
				std::remove( m_neighbours.begin(), m_neighbours.end(), ( *i ) );
				removed++;
				m_delegate.OnNeighbourPurged( *this, i->second );
			}
		}
		
		if( removed > 0 )
		{
			m_neighbours.erase( m_neighbours.end() - removed, m_neighbours.end() );
		}
	}
	
	RDSize NeighbourList::Count() const
	{
		return m_neighbours.size();
	}

	BloomFilter NeighbourList::Neighbours( const RDSize count ) const
	{
		BloomFilter neighbours( count, BloomFilter::DesiredFalsePositiveRate );
		
		for( 	std::vector< std::pair<RDTimeStamp, Node> >::const_iterator i = m_neighbours.begin();
			i != m_neighbours.end(); ++i )
		{
			neighbours.Insert( i->second.Address() );
		}
		
		return neighbours;
	}
	
	std::set<Node> NeighbourList::CalculateMPRNodes() const
	{
		BloomFilter filter, u, mprFilter;
		std::set<BloomFilter> neighbours, mprs;
		std::set<Node> mprNodes;
		
		for( 	std::vector< std::pair<RDTimeStamp, Node> >::const_iterator i = m_neighbours.begin();
			i != m_neighbours.end(); ++i )
		{
			u = u.Union( i->second.Neighbours() );
			neighbours.insert( i->second.Neighbours() );
		}
		
		for( 	std::vector< std::pair<RDTimeStamp, Node> >::const_iterator i = m_neighbours.begin();
			i != m_neighbours.end(); ++i )
		{
			u.Remove( i->second.Address() );
		}

		FindUniqueRelations( neighbours, mprs );
		
		for( std::set<BloomFilter>::const_iterator i = mprs.begin(); i != mprs.end(); ++i )
		{
			neighbours.erase( ( *i ) );
			filter = filter.Union( ( *i ) );
		}
		
		for( std::set<BloomFilter>::reverse_iterator i = neighbours.rbegin(); i != neighbours.rend(); ++i )
		{
			if( u.Difference( filter ) == 0 )
			{
				break;
			}
			
			if( filter.Difference( ( *i ) ) != 0 && filter.Contains( ( *i ) ) == false )
			{
				mprs.insert( ( *i ) );
				filter = filter.Union( ( *i ) );
			}
		}
		
		for( std::set<BloomFilter>::reverse_iterator i = neighbours.rbegin(); i != neighbours.rend(); ++i )
		{
			if( filter.RemovalResultsInDifference( *i ) == false )
			{
				mprs.erase( ( *i ) );
				filter.Remove( ( *i ) );
			}
		}
		
		for( std::set<BloomFilter>::const_iterator i = mprs.begin(); i != mprs.end(); ++i )
		{
			for( 	std::vector< std::pair< RDTimeStamp, Node > >::const_iterator j = m_neighbours.begin();
				j != m_neighbours.end(); ++j )
			{
				if( ( *i ) == j->second.Neighbours() )
				{
					mprNodes.insert( j->second );
					break;
				}
			}
		}
		
		return mprNodes;
	}
	
	std::set<Node> NeighbourList::NeighbourSet() const
	{
		std::set<Node> neighbours;
	
		for( 	std::vector< std::pair< RDTimeStamp, Node > >::const_iterator j = m_neighbours.begin();
				j != m_neighbours.end(); ++j )
		{
			neighbours.insert( j->second );
		}
		
		return neighbours;
	}
	
	void NeighbourList::FindUniqueRelations( 	const std::set<BloomFilter>& neighbours, 
							std::set<BloomFilter>& uniques ) const
	{
		for( std::set<BloomFilter>::const_iterator i = neighbours.begin(); i != neighbours.end(); ++i )
		{
			bool unique = true;
			
			for( std::set<BloomFilter>::const_iterator j = neighbours.begin(); j != neighbours.end(); ++j )
			{
				if( j->Contains( *i ) )
				{
					unique = false;
					break;
				}
			}
			
			if( unique )
			{
				uniques.insert( ( *i ) );
			}
		}
	}
} }

