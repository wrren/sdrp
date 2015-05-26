/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_NEIGHBOUR_LIST_DELEGATE_H
#define RD_SDRP_NEIGHBOUR_LIST_DELEGATE_H

#include <SDRP/Routing/Node.h>

namespace Radicle { namespace SDRP
{
	/// NeighbourList Forward-Declaration
	class NeighbourList;

	/**
	 *	Delegate for Neighbour List Events
	 */
	class NeighbourListDelegate
	{
	public:
	
		/**
		 *	The specified neighbour was purged due to a lack of beacons
		 * @param sender	Sending Neighbour List
		 * @param neighbour	Purged Neighbour
		 */
		virtual void OnNeighbourPurged( NeighbourList& sender, const Node& neighbour ) {};
		
		/**
		 *	The specified node was added as a new neighbour
		 * @param sender	Sending Neighbour List
		 * @param neighbour	New Neighbour
		 */
		virtual void OnNeighbourAdded( NeighbourList& sender, const Node& neighbour ) {};
	
	private:
	
	};
} }

#endif // RD_SDRP_NEIGHBOUR_LIST_DELEGATE_H

