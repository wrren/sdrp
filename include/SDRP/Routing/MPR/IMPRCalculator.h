/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_IMPR_CALCULATOR_H
#define RD_SDRP_IMPR_CALCULATOR_H

#include <SDRP/Routing/Node.h>

namespace Radicle { namespace SDRP 
{
	/**
	 *	Base class for MPR calculators
	 */
	class IMPRCalculator
	{
	public:

		IMPRCalculator( const Node& localNode ) :
		m_localNode( localNode )
		{}

		/// Node Container Type
		typedef std::set<Node> NodeContainer;

		/**
		 *	Calculate a set of MPR Nodes using the given neighbour information
		 * @param neighbours 	Current Node Neighbours
		 * @return 		Bloom Filter Describing MPR Nodes
		 */
		virtual BloomFilter Calculate( const NodeContainer& neighbours ) = 0;

	protected:

		Node m_localNode;
	};
}}

#endif // RD_SDRP_IMPR_CALCULATOR_H
