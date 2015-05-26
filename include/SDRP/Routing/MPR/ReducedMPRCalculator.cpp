/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#include <SDRP/Routing/MPR/ReducedMPRCalculator.h>
#include <cmath>

namespace Radicle { namespace SDRP {
	
	ReducedMPRCalculator::RMPRContainer::RMPRContainer( const Radicle::SDRP::Node& node, float difference ) : 	m_node( node ),
															m_difference( difference )
	{}

	void ReducedMPRCalculator::RMPRContainer::Node( const Radicle::SDRP::Node& node  )
	{
		m_node = node;
	}
	
	const Radicle::SDRP::Node& ReducedMPRCalculator::RMPRContainer::Node() const
	{
		return m_node;
	}

	void ReducedMPRCalculator::RMPRContainer::Difference( float difference  )
	{
		m_difference = difference;
	}
	
	float ReducedMPRCalculator::RMPRContainer::Difference() const
	{
		return m_difference;
	}

	bool ReducedMPRCalculator::RMPRContainer::operator<( const RMPRContainer& other ) const
	{
		return m_difference > other.m_difference ? false : true;
	}

	bool ReducedMPRCalculator::RMPRContainer::operator>( const RMPRContainer& other ) const
	{
		return !( ( *this ) < other );
	}

	bool ReducedMPRCalculator::RMPRContainer::operator==( const RMPRContainer& other ) const
	{
		return m_node.Address() == other.m_node.Address();
	}

	bool ReducedMPRCalculator::RMPRContainer::operator!=( const RMPRContainer& other ) const
	{
		return !( ( *this ) == other );
	}

	IMPRCalculator::NodeContainer ReducedMPRCalculator::EliminateSimilarNodes( 	const IMPRCalculator::NodeContainer& input, 
											float minimumDifference )
	{
		NodeContainer container( input );

		for( NodeContainer::iterator i = container.begin(); i != container.end(); )
		{
			NodeContainer::iterator start( i );
			start++;

			if( i->Neighbours().TableSize() == 0 )
			{
				container.erase( i++ );
				continue;
			}

			for( NodeContainer::iterator j = start; j != container.end(); )
			{
				if( i->Neighbours().TableSize() == j->Neighbours().TableSize() )
				{
					double difference = i->Neighbours().SetDifference( j->Neighbours() );

					if( difference / i->Neighbours().TableSize() < minimumDifference )
					{
						container.erase( j++ );
						continue;
					}
				}
				j++;
			}

			i++;
		}

		return container;
	}

	float ReducedMPRCalculator::Mean( const std::set<ReducedMPRCalculator::RMPRContainer>& differenceSet )
	{
		if( differenceSet.size() == 0 )
		{
			return 0;
		}

		float sum = 0.0f;

		for( std::set<RMPRContainer>::const_iterator i = differenceSet.begin(); i != differenceSet.end(); i++ )
		{
			sum += i->Difference();
		}

		return sum / differenceSet.size();
	}

	float ReducedMPRCalculator::StdDev( const std::set<ReducedMPRCalculator::RMPRContainer>& differenceSet )
	{
		if( differenceSet.size() == 0 )
		{
			return 0;
		}

		float mean = Mean( differenceSet );
		float sum = 0.0f;

		for( std::set<RMPRContainer>::const_iterator i = differenceSet.begin(); i != differenceSet.end(); i++ )
		{
			sum += pow( i->Difference() - mean, 2 );
		}

		return sqrt( sum ) / differenceSet.size();
	}

	BloomFilter ReducedMPRCalculator::Calculate( const IMPRCalculator::NodeContainer& neighbours )
	{
		// Create a set of nodes which can be sorted by degree of difference w.r.t neighbour filters
		std::set<RMPRContainer> differenceSet;
		BloomFilter mprNodes;

		NodeContainer reduced = EliminateSimilarNodes( neighbours, 0.05 );

		// Fill the set
		for( NodeContainer::const_iterator i = reduced.begin(); i != reduced.end(); i++ )
		{
			if( i->Neighbours().TableSize() > 0 )
			{
				BloomFilter neighbourFilter( i->Neighbours() );
				RMPRContainer neighbourDescription( ( *i ) );

				// Create comparison filter
				BloomFilter myNeighbours( neighbourFilter.TableSize(), neighbourFilter.HashCount() );

				myNeighbours.Insert( m_localNode.Address() );

				for( NodeContainer::const_iterator j = neighbours.begin(); j != neighbours.end(); j++ )
				{
					if( j->Address() == i->Address() )
					{
						continue;
					}

					myNeighbours.Insert( j->Address() );
				}

				float difference = ( ( float ) myNeighbours.SetDifference( neighbourFilter ) );
				if( difference > 0 )
				{
					neighbourDescription.Difference( difference );
					differenceSet.insert( neighbourDescription );
				}
				
			}
		}

		float upperBound = floor( Mean( differenceSet ) + StdDev( differenceSet ) );

		for( std::set<RMPRContainer>::iterator i = differenceSet.begin(); i != differenceSet.end(); )
		{
			if( i->Difference() < upperBound )
			{
				differenceSet.erase( i++ );
			}
			else
			{
				i++;
			}
		}

		if( differenceSet.size() > 0 )
		{
			mprNodes.ResetWithParameters( differenceSet.size(), ( ( float ) differenceSet.size() ) / 10 );
		}
		else
		{
			mprNodes.DestructiveResize( 1, 0 ).Universe();
		}

		// Perform rotation filter
		for( std::set<RMPRContainer>::iterator i = differenceSet.begin(); i != differenceSet.end(); )
		{
			std::set<RMPRContainer>::iterator j( i );
			j++;
			
			for( ; j != differenceSet.end(); j++ )
			{
				if( i->Node().Neighbours().Contains( j->Node().Address() ) == false )
				{
					i = j;
					break;
				}
			}

			mprNodes.Insert( i->Node().Address() );
			
			if( j == differenceSet.end() )
			{
				i++;
			}
		}

		return mprNodes;
	}
} }