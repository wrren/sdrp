/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_ROUTE_TABLE_H
#define RD_SDRP_ROUTE_TABLE_H

#include <SDRP/Core/Core.h>
#include <SDRP/Routing/Route.h>

namespace Radicle { namespace SDRP
{
	class RouteTable
	{
	public:
	
		/**
		 *	Default Constructor
		 */
		RouteTable();
	
		/**
		 *	Add a new route to the route table
		 * @param newRoute	New Route
		 */
		void Add( const Route& newRoute );
	
		/**
		 *	Purge all routes from the table with an age greater than maxAge
		 * @param maxAge	Maximum Route Age
		 */
		void Purge( const RDTimeStamp maxAge );
		
		/**
		 *	Check whether a route to the specified service is available
		 * @param service	Service
		 * @return		True - If a route to the specified service is available.
		 *			False otherwise.
		 */
		bool HasRoutesToService( const RDServiceIdentifier service ) const;
		
		/**
		 *	Get a set of routes leading to the specified service	
		 * @param service	Service
		 * @return		All Available Routes to Service
		 */
		std::set<Route> RoutesToService( const RDServiceIdentifier service ) const;
	
		/**
		 *	Check whether a route to the specified host is available
		 * @param host		Host
		 * @return		True - If a route to the specified host is available.
		 *			False otherwise.
		 */
		bool HasRouteToHost( const RDNetworkAddress host ) const;
	
		/**
		 *	Get all routes to the specified address
		 * @param address	Target network address
		 * @return		Routes to requested address
		 */
		std::set<Route> RoutesToHost( const RDNetworkAddress address ) const;
	
		/**
		 *	Check whether a route superior to the provided route exists in the
		 *	route table
		 * @param newRoute	Route to be evaluated
		 * @return		True - If a superior route exists in the routing table. False otherwise.
		 */
		bool HaveSuperiorRoute( const Route& newRoute ) const;
	
	private:
	
		/// Convenience Typedef
		typedef std::list<Route>	RouteList;
	
		/// Routes
		RouteList	m_routes;
	};
} }

#endif // RD_SDRP_ROUTE_TABLE_H

