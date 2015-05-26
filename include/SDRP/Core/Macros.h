/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_MACROS_H
#define RD_SDRP_MACROS_H

#include <SDRP/Utilities/Logger.h>
#include <iostream>
#include <cstdlib>

#ifdef RD_DEBUG_ENABLED

#define		RD_CEVENT(c,m)			Radicle::SDRP::Logger::Event( __FILE__, __LINE__, c ) << m << std::endl
#define		RD_EVENT(m)			Radicle::SDRP::Logger::Event( __FILE__, __LINE__ ) << m << std::endl				
#define		RD_ASSERT(c,code,m)		if( ( c ) == false ) \
						{ \
							Radicle::SDRP::Logger::Error( __FILE__, __LINE__, code ) << m << std::endl; \
							std::cout << "ASSERTION FAILURE " << __FILE__ << ":" << __LINE__ << " - " << m << std::endl; \
							exit( 1 ); \
						}					
#define 	RD_RESULT(m)			Radicle::SDRP::Logger::Result() << m << std::endl
#define		RD_ERROR(c,m)			Radicle::SDRP::Logger::Error( __FILE__, __LINE__, c ) << m << std::endl
#define		RD_NODE(n)			Radicle::SDRP::Logger::CurrentNode( static_cast<RDUInt32>( n ) )
#define		RD_NLOG(m)			Radicle::SDRP::Logger::Node( __FILE__, __LINE__ ) << m << std::endl
#define		RD_PRINT(m)			std::cout << __FILE__ << ":" << __LINE__ << " - " << m << std::endl;
	
#else

#define		RD_CEVENT(c,m)
#define		RD_EVENT(m)	
#define		RD_ASSERT(c,code,m)		
#define 	RD_RESULT(m)
#define		RD_ERROR(c,m)
#define		RD_NODE(n)
#define		RD_NLOG(m)
#define 	RD_PRINT(m)	

#endif
	
#endif // RD_SDRP_MACROS_H
 
