/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_EXCEPTION_H
#define RD_SDRP_EXCEPTION_H

namespace Radicle { namespace SDRP
{

	/**
	 *	Base Exception Class
	 */
	class Exception
	{
	public:

		/**
		 *	Get the message associated with this exception
		 * @return 	Exception Message
		 */
		virtual const char* Message() const throw() = 0;
	};
} }

#endif // RD_SDRP_EXCEPTION_H
