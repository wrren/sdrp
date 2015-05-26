/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_LOCAL_AREA_MONITOR_H
#define RD_SDRP_LOCAL_AREA_MONITOR_H

#include <SDRP/Core/Core.h>
#include <SDRP/Routing/Node.h>
#include <SDRP/Routing/MPR/MPRFactory.h>

namespace Radicle { namespace SDRP 
{
	/**
	 *	Listener for Local Area Change Events
	 */
	class LocalAreaListener
	{
	public:

		/**
		 *	A new neighbour was seen
		 * @param neighbour 	New Neighbour
		 */
		virtual void OnNeighbourAdded( const Node& neighbour ) = 0;

		/**
		 *	Contact with a neighbour was lost
		 * @param neighbour 	Lost Neighbour
		 */
		virtual void OnNeighbourLost( const Node& neighbour ) = 0;
	};

	/**
	 *	Monitors local node area, recording neighbour information
	 */
	class LocalAreaMonitor : public Publisher<LocalAreaListener>
	{
	public:

		/// Default Max Neighbour Age
		static const RDTime DefaultMaxAge;

		/// Node Container Type
		typedef std::set<Node>	NodeContainer;


		/**
		 *	Default Constructor
		 */
		LocalAreaMonitor( const Node& localNode );

		/**
		 *	Calculate the MPR Filter for Service Advertisements
		 */
		const BloomFilter& MPRFilter();

		/**
		 *	Set the MPR Selection Mode
		 */
		void Mode( MPRFactory::MPRSelectionMode mode );

		/**
		 *	Get the MPR Selection Mode
		 */
		const MPRFactory::MPRSelectionMode Mode() const;

		/**
		 *	Get a bloom filter describing this node's neighbours
		 * @param constrain 	Indicates whether the bloom filter should be constrained to a certain size
		 * @param size 		Constraint Size
		 */
		const BloomFilter& NeighbourFilter( bool constrain = false, const RDSize size = 0 );

		/**
		 *	Set the maximum neighbour age. This will determine when neighbours are purged
		 *	due to inactivity
		 * @param max 	Maximum neighbour age in seconds
		 */
		void MaxNeighbourAge( const RDTime max );

		/**
		 *	Get the maximum neighbour age. This determines when neighbours are purged
		 *	due to inactivity
		 * @return 	Maximum neighbour age in seconds
		 */
		RDTime MaxNeighbourAge() const;

		/**
		 *	A node was seen in the local area
		 * @param node 	Seen Node
		 */
		void NodeWasSeen( const Node& node );

		/**
		 *	Purge any neighbour that hasn't been seen recently
		 */
		void Purge();

	protected:

		/**
		 *
		 * @return 	Bloom Filter containing all neighbour addresses
		 */
		void RebuildNeighbourFilter();

		/// MPR Selection Mode
		MPRFactory::MPRSelectionMode	m_mode;
		/// MPR Calculator Factory
		MPRFactory			m_calculatorFactory;
		/// MPR Calculator
		MPRFactory::MPRCalculatorPtr 	m_calculator;
		/// Max Neighbour Age
		RDTime 				m_maxAge;
		/// Neighbours
		NodeContainer			m_neighbours;
		/// Cached MPR State Validity
		bool 				m_cacheIsValid;
		/// Cached MPR Filter
		BloomFilter 			m_mpr;
		/// Neighbour Filter
		BloomFilter			m_neighbourFilter;
	};
}}

#endif // RD_SDRP_LOCAL_AREA_MONITOR_H
