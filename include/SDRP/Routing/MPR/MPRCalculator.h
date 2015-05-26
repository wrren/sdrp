/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_MPR_CALCULATOR_H
#define RD_SDRP_MPR_CALCULATOR_H

#include <SDRP/Routing/MPR/IMPRCalculator.h>

namespace Radicle { namespace SDRP {
	/**
	 *	Standard OLSR-Algorithm MPR Calculator
	 */
	class MPRCalculator : public IMPRCalculator
	{
	public:

		MPRCalculator( const Node& node ) : IMPRCalculator( node ) {}

		/**
		 *	Calculate a set of MPR Nodes using the given neighbour information
		 * @param neighbours 	Current Node Neighbours
		 * @return 		Bloom Filter Describing MPR Nodes
		 */
		virtual BloomFilter Calculate( const std::set<Node>& neighbours );
	};
}}

#endif // RD_SDRP_MPR_CALCULATOR_H
