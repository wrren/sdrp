/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/SDRP.h>
 
namespace Radicle { namespace SDRP
{
	SDRP::SDRP( SDRPDelegate& delegate ) :
	m_delegate( delegate ),
	m_manager( delegate, m_node )
	{
		Logger::Initialize( delegate );
	}
	
	const Node& SDRP::LocalNode() const
	{
		return m_node;
	}
	
	Node& SDRP::LocalNode()
	{
		return m_node;
	}
	
	void SDRP::LocalNode( const Node& var )
	{
		m_node = var;
	}
	
	const RoutingManager& SDRP::Routing() const
	{
		return m_manager;
	}
	
	RoutingManager& SDRP::Routing()
	{
		return m_manager;
	}
} }
 
