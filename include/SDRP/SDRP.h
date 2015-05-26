/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_H
#define RD_SDRP_H

#include <SDRP/Core/Core.h>
#include <SDRP/Routing/RoutingManager.h>

namespace Radicle { namespace SDRP
{
	/**
	 *	Main Interface for SDRP Protocol
	 */
	class SDRP
	{
	public:
	
		/**
		 *	Default Constructor
		 */
		SDRP( SDRPDelegate& delegate );
		
		/**
		 *	Get the local SDRP node
		 * @return	Local SDRP node
		 */
		const Node& LocalNode() const;
		
		/**
		 *	Get the local SDRP node
		 * @return	Local SDRP node
		 */
		Node& LocalNode();
		
		/**
		 *	Set the local SDRP node
		 * @param	node	local SDRP node 
		 */
		void LocalNode( const Node& node );
		
		/**
		 *	Get the SDRP routing manager
		 * @return	Routing Manager
		 */
		const RoutingManager& Routing() const;
		
		/**
		 *	Get the SDRP routing manager
		 * @return	Routing Manager
		 */
		RoutingManager& Routing();
			
	private:
		
		/// SDRP Delegate
		SDRPDelegate&	m_delegate;
		/// Local Node
		Node		m_node;
		/// Routing Manager
		RoutingManager	m_manager;
	};
} }

#endif // RD_SDRP_H
 
