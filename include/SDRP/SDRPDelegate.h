/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_DELEGATE_H
#define RD_SDRP_DELEGATE_H

#include <SDRP/Core/Types.h>
#include <string>

namespace Radicle { namespace SDRP
{
	/**
	 *	Delegate for the SDRP Protocol. Used for sending packets and
	 *	ascertaining the current time.
	 */
	class SDRPDelegate
	{
	public:
		
		/**	
		 *	Instructs the delegate to send the provided packet to the specified network address
		 * @param packetData	Packet data to be sent
		 * @param packetSize	Size of packet data
		 * @param destination	Destination network address
		 */
		virtual void Send( 	const RDUByte8* packetData, 
					const RDSize packetSize, 
					const RDNetworkAddress destination ) = 0;
					
		/**
		 *	Get the current time
		 * @return	Current Time
		 */
		virtual RDTimeStamp Time() const = 0;
		
		/**
		 *	Get the name of this delegate
		 * @return	Delegate Name
		 */
		virtual std::string Name() const = 0;
	};
} }

#endif // RD_SDRP_DELEGATE_H
