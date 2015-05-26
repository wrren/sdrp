/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Routing/RouteTable.h>
#include <sstream>

namespace Radicle { namespace SDRP
{
	RouteTable::RouteTable()
	{}
	
	void RouteTable::Add( const Route& newRoute )
	{	
		for( RouteList::iterator i = m_routes.begin(); i != m_routes.end(); ++i )
		{
			if( i->Server() == newRoute.Server() && i->NextHop() == newRoute.NextHop() )
			{
				( *i ) = newRoute;
				return;
			}
		}
		
		m_routes.push_back( newRoute );
		
		std::stringstream stream;
		
		for( RDServiceIdentifier i = 0; i < 10; i++ )
		{
			if( newRoute.Services().Contains( i ) )
			{
				stream << i << ", ";
			}
		}
		
		RDUInt32 hops = newRoute.Hops();
		
		RD_NLOG( 	"Have Route to Service Filter with Server " << newRoute.Server() << " in " << hops << 
				" Hops through Node " << newRoute.NextHop() << " with Services: " << stream.str() );
	}
	
	void RouteTable::Purge( const RDTimeStamp maxAge )
	{
		RDTimeStamp time = Logger::Time();
	
		for( 	RouteList::iterator i = m_routes.begin();
			i != m_routes.end(); )
		{
			if( ( time - i->Age() ) > maxAge )
			{
				m_routes.erase( i++ );
			}
			else
			{
				++i;
			}
		}
	}
	
	bool RouteTable::HasRoutesToService( const RDServiceIdentifier service ) const
	{
		for( RouteList::const_iterator i = m_routes.begin(); i != m_routes.end(); ++i )
		{
			if( i->Services().Contains( service ) )
			{
				return true;
			}
		}
		
		return false;
	}
	
	std::set<Route> RouteTable::RoutesToService( const RDServiceIdentifier service ) const
	{
		std::set<Route> routes;
		
		for( RouteList::const_iterator i = m_routes.begin(); i != m_routes.end(); ++i )
		{
			if( i->Services().Contains( service ) )
			{
				routes.insert( ( *i ) );
			}
		}
		
		if( routes.size() == 0 )
		{
			RD_NLOG( "Failed to Find Route to Service " << service << " Route Table (" << m_routes.size() << ") Dump Follows" );
			
			for( RouteList::const_iterator i = m_routes.begin(); i != m_routes.end(); ++i )
			{
				std::stringstream stream;
				
				stream << "Server " << i->Server() << ": ";
			
				for( RDServiceIdentifier s = 0; s < 10; s++ )
				{
					if( i->Services().Contains( s ) )
					{
						stream << s << ", ";
					}
				}
				
				RD_NLOG( stream.str() );
			}
		}
		
		return routes;
	}
	
	bool RouteTable::HasRouteToHost( const RDNetworkAddress address ) const
	{
		for( RouteList::const_iterator i = m_routes.begin(); i != m_routes.end(); ++i )
		{
			if( i->Server() == address )
			{
				return true;
			}
		}
		
		return false;
	}
	
	std::set<Route> RouteTable::RoutesToHost( const RDNetworkAddress address ) const
	{
		std::set<Route> routes;
		
		for( RouteList::const_iterator i = m_routes.begin(); i != m_routes.end(); ++i )
		{
			if( i->Server() == address )
			{
				routes.insert( ( *i ) );
			}
		}
		
		return routes;
	}
	
	bool RouteTable::HaveSuperiorRoute( const Route& newRoute ) const
	{
		std::set<Route> routes;
		
		for( RouteList::const_iterator i = m_routes.begin(); i != m_routes.end(); ++i )
		{
			if( 	i->Server() != newRoute.Server() && 
				i->Services().Contains( newRoute.Services() ) &&
				i->Hops() < newRoute.Hops() )
			{
				return true;
			}
		}
		
		return false;
	}
} }

