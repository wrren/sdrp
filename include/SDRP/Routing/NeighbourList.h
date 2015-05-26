/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_NEIGHBOUR_LIST_H
#define RD_SDRP_NEIGHBOUR_LIST_H

#include <SDRP/Core/Core.h>
#include <SDRP/Routing/Node.h>
#include <SDRP/Routing/NeighbourListDelegate.h>

namespace Radicle { namespace SDRP
{
	class NeighbourList
	{
	public:
	
		class Iterator
		{
		public:
		
			Iterator( std::vector< std::pair<RDTimeStamp, Node> >::const_iterator i );
		
			const Node* operator->() const
			{
				return &( m_i->second );
			}
			
			bool operator==( const Iterator& other ) const
			{
				return other.m_i == m_i;
			}
			
			bool operator!=( const Iterator& other ) const
			{
				return !( *this == other );
			}
			
			Iterator operator++( int i ) const
			{
				return Iterator( m_i + i );
			}
			
		protected:
		
			std::vector< std::pair<RDTimeStamp, Node> >::const_iterator m_i;
		};
	
		/**
		 *	Default Constructor
		 * @param delegate	List Delegate
		 * @param localNode	Local Node Data
		 */
		NeighbourList( NeighbourListDelegate& delegate, Node& localNode );
	
		/**
		 *	Add a neighbour to the list.
		 * @param neighbour	Neighbour to be added
		 */
		void Add( const Node& neighbour );
		
		/**
		 *	Purge all neighbours with an age greater than \a maxAge from the list
		 * @param maxAge	Maximum Neighbour Age
		 */
		void Purge( const RDTimeStamp maxAge );

		/**
		 *	Get the number of neighbours registered with the neighbour list
		 * @return 	Neighbour Count
		 */
		RDSize Count() const;
		
		/**
		 *	Get a bloom filter composed of all neighbour addresses
		 * @param 	Expected Neighbour Count
		 * @return	Neighbour Address Bloom Filter
		 */
		BloomFilter Neighbours( const RDSize count = BloomFilter::PredictedElementCount ) const;
		
		/**
		 *	Calculate MPRs based on the current neighbour list
		 * @return	MPR Node Set
		 */
		std::set<Node> CalculateMPRNodes() const;
		
		/**
		 *	Get the set of all current neighbour nodes
		 * @return	Set of all current neighbour nodes
		 */
		std::set<Node> NeighbourSet() const;
		
		Iterator Begin() const
		{
			return Iterator( m_neighbours.begin() );
		}
		
		Iterator End() const
		{
			return Iterator( m_neighbours.end() );
		}
		
	private:
	
		/**
		 *	Find all neighbours which act as the only link to a two-hop neighbour
		 * @param neighbours	Neighbour Node Neighbour Filters
		 * @param uniques[out]	Uniquely Connected Neighbours
		 */
		void FindUniqueRelations( 	const std::set<BloomFilter>& neighbours, 
						std::set<BloomFilter>& uniques ) const;


		/// Neighbours
		std::vector< std::pair<RDTimeStamp, Node> > m_neighbours;	
		
		/// The Local Node
		Node&			m_localNode;
		
		/// List Delegate
		NeighbourListDelegate&	m_delegate;
	};
} }

#endif // RD_SDRP_NEIGHBOUR_LIST_H

