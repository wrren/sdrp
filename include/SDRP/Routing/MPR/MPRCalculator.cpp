/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#include <SDRP/Routing/MPR/MPRCalculator.h>

namespace Radicle { namespace SDRP {
	
	class NeighbourSortComparator
	{
	public:

		NeighbourSortComparator( const BloomFilter& localNeighbours ) :
		m_localNeighbours( localNeighbours )
		{}

		/**
		 *	Compare two Nodes based on their neighbour filter
		 * @return 	True if a should be placed before b. False otherwise.
		 */
		bool operator()( const Node& a, const Node& b )
		{
			BloomFilter aBloom( a.Neighbours() );
			BloomFilter bBloom( b.Neighbours() );
			aBloom.Remove( m_localNeighbours );
			bBloom.Remove( m_localNeighbours );

			return aBloom.SetBytes() > bBloom.SetBytes();
		}

	private:

		BloomFilter m_localNeighbours;
	};
	
	BloomFilter MPRCalculator::Calculate( const std::set<Node>& neighbours )
	{
		BloomFilter 			requiredCoverage;		// Required 1-Hop Address Coverage
		BloomFilter 			coverage;			// Calculated Coverage Area
		std::set<RDNetworkAddress> 	mprAddresses;			// MPR Node Addresses
		BloomFilter			neighbourFilter;		// Filter containing all neighbour identifiers

		// Generate Bloom Filter for required neighbour coverage
		for( NodeContainer::const_iterator i = neighbours.begin(); i != neighbours.end(); i++ )
		{
			requiredCoverage = requiredCoverage.Union( i->Neighbours() );
		}

		NeighbourSortComparator comparator( neighbourFilter );
		std::set<Node, NeighbourSortComparator> sizeSet( comparator );

		// Find Nodes with Unique Connections
		for( NodeContainer::const_iterator i = neighbours.begin(); i != neighbours.end(); i++ )
		{
			Node n( *i );
			n.Neighbours().Remove( neighbourFilter );

			if( requiredCoverage.RemovalResultsInDifference( n.Neighbours() ) )
			{
				mprAddresses.insert( n.Address() );
				coverage = coverage.Union( n.Neighbours() );
			}
			else
			{
				sizeSet.insert( n );
			}
		}

		// Add nodes in order of coverage area
		for( 	std::set<Node, NeighbourSortComparator>::const_iterator i = sizeSet.begin(); 
			i != sizeSet.end() && coverage.Difference( requiredCoverage ) > 0; i++ )
		{
			if( coverage.Contains( i->Neighbours() ) == false )
			{
				coverage = coverage.Union( i->Neighbours() );
				mprAddresses.insert( i->Address() );
			}
		}

		// Create an MPR Filter 
		BloomFilter mprNodes( mprAddresses.size() * 2, BloomFilter::DesiredFalsePositiveRate );

		// Insert MPR Node Addresses
		for( std::set<RDNetworkAddress>::const_iterator i = mprAddresses.begin(); i != mprAddresses.end(); i++ )
		{
			mprNodes.Insert( ( *i ) );
		}

		return mprNodes;
	}
}}