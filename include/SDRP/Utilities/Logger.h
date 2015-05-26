/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_LOGGER_H
#define RD_SDRP_LOGGER_H

#include <SDRP/SDRPDelegate.h>
#include <SDRP/Core/Types.h>
#include <SDRP/Core/Definitions.h>
#include <iostream>
#include <map>

namespace Radicle { namespace SDRP
{
	class Logger
	{
	public:
		
		/**
		 *	Initialize the logger with the provided delegate
		 * @param delegate	SDRP Delegate
		 */
		static void Initialize( SDRPDelegate& delegate );
		
		/**
		 *	Get an output stream for writing to the event log
		 * @param file	File in which the event occurred
		 * @param line	Line on which the event occurred
		 * @param code	Event Code
		 * @return	Event Log Output Stream
		 */
		static std::ostream& Event( const RDChar8* file, RDUInt16 line, RDUInt16 code = 0 );
		
		/**
		 *	Get an output stream for writing to the error log
		 * @param file	File in which the error occurred
		 * @param line	Line on which the error occurred
		 * @param code	Error Code
		 * @return	Error Log Output Stream
		 */
		static std::ostream& Error( const RDChar8* file, RDUInt16 line, RDUInt16 code = 0 );
		
		/**
		 *	Get an output stream for writing to the node's event log
		 * @param file	File in which the event occurred
		 * @param line	Line on which the event occurred
		 * @param code	Event Code
		 * @return	Event Log Output Stream
		 */
		static std::ostream& Node( const RDChar8* file, RDUInt16 line, RDUInt16 code = 0 );
		
		/**
		 *	Get an output stream for writing to the results log
		 * @return	Results Log Output Stream
		 */
		static std::ostream& Result();
		
		/**
		 *	Get the current time
		 * @return	Current Time
		 */
		static RDTimeStamp Time();
		
		/**
		 *	Set the currently logging node
		 * @param node	Address/ID of currently logging node
		 */
		static void CurrentNode( const RDUInt32 node = RD_SDRP_UNSPECIFIED_ADDRESS );
		
	private:

		/**
		 *	Default Constructor
		 * @param delegate	SDRP Delegate
		 */
		Logger( SDRPDelegate& delegate );

		/**
		 *	Get the current time
		 */
		RDTimeStamp CurrentTime() const;

		/**
		 *	Get the Logger Singleton
		 * @return	Logger Singleton
		 */
		static Logger* Instance();

		/**
		 *	Get the output stream for the current node
		 * @return	Output Stream
		 */
		std::ostream& NodeLog();
	
		/// Static Logger Instance
		static Logger* m_instance;
		/// SDRP Delegate
		SDRPDelegate&					m_delegate;
		/// Event Log Output Stream
		std::ostream*					m_eventLog;
		/// Error Log Output Stream
		std::ostream*					m_errorLog;	
		/// Result Log Output Stream
		std::ostream*					m_resultLog;
		/// Node Logs
		std::map<RDNetworkAddress, std::ostream* >	m_nodeLogs;
		/// Currently Logging Node
		RDNetworkAddress				m_currentNode;
	};
} }

#endif // RD_SDRP_LOGGER_H
 
