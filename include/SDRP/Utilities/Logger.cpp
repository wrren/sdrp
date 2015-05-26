/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Utilities/Logger.h>
#include <SDRP/SDRPDelegate.h>
#include <sstream>
#include <fstream>

namespace Radicle { namespace SDRP
{
	Logger* Logger::m_instance = NULL;

	Logger::Logger( SDRPDelegate& delegate ) :
	m_delegate( delegate ),
	m_eventLog( NULL ),
	m_errorLog( NULL ),
	m_resultLog( NULL ),
	m_currentNode( RD_SDRP_UNSPECIFIED_ADDRESS )
	{
		
	}

	std::ostream& Logger::Event( const RDChar8* file, RDUInt16 line, RDUInt16 code )
	{
		Logger* instance = Logger::Instance();
		
		if( instance != NULL )
		{
			if( instance->m_eventLog == NULL ) 
			{
				instance->m_eventLog = new std::ofstream( ( 	instance->m_delegate.Name() + ".event.log" ).c_str(), 
										std::ofstream::trunc );
			}
		
			std::ostream& log = *( Logger::Instance()->m_eventLog );
			
			log << std::endl ;
			log << "Time: " << instance->CurrentTime() << std::endl;
			log << "File: " << file << std::endl;
			log << "Line: " << line << std::endl;
			log << "Code: " << code << std::endl;
			
			return log;
		}
		else
		{
			std::cout << std::endl;
			std::cout << "File: " << file << std::endl;
			std::cout << "Line: " << line << std::endl;
			std::cout << "Code: " << code << std::endl;
			
			return std::cout;
		}
	}
		
	std::ostream& Logger::Error( const RDChar8* file, RDUInt16 line, RDUInt16 code )
	{
		Logger* instance = Logger::Instance();
		
		if( instance != NULL )
		{
			if( instance->m_errorLog == NULL )
			{
				instance->m_errorLog = new std::ofstream( ( 	instance->m_delegate.Name() + ".error.log" ).c_str(), 
										std::ofstream::trunc );
			}
		
			std::ostream& log = *( instance->m_errorLog );
			
			log << std::endl ;
			log << "Time: " << instance->CurrentTime() << std::endl;
			log << "File: " << file << std::endl;
			log << "Line: " << line << std::endl;
			log << "Code: " << code << std::endl;
			
			return log;
		}
		else
		{
			std::cout << std::endl;
			std::cout << "File: " << file << std::endl;
			std::cout << "Line: " << line << std::endl;
			std::cout << "Code: " << code << std::endl;
			
			return std::cout;
		}
	}
	
	std::ostream& Logger::Result()
	{
		Logger* instance = Logger::Instance();
		
		if( instance != NULL )
		{
			if( instance->m_resultLog == NULL )
			{
				instance->m_resultLog 	= new std::ofstream( ( 	instance->m_delegate.Name() + ".result.log" ).c_str(), 
										std::ofstream::trunc );	
			}
			std::ostream& log = *( instance->m_resultLog );
			return log;
		}
		else
		{
			return std::cout;
		}
	}

	std::ostream& Logger::Node( const RDChar8* file, RDUInt16 line, RDUInt16 code )
	{
		Logger* instance = Logger::Instance();
		
		if( instance != NULL )
		{
			std::ostream& log = instance->NodeLog();
			
			log << "Time: " << instance->CurrentTime() << " - ";
			
			return log;
		}
		else
		{
			std::cout << std::endl;
			std::cout << "File: " << file << std::endl;
			std::cout << "Line: " << line << std::endl;
			std::cout << "Code: " << code << std::endl;
			
			return std::cout;
		}	
	}

	RDTimeStamp Logger::Time()
	{
		return m_instance != NULL ? Logger::Instance()->CurrentTime() : 0;
	}

	void Logger::CurrentNode( const RDUInt32 node )
	{
		Logger::Instance()->m_currentNode = node;
	}

	std::ostream& Logger::NodeLog()
	{
		std::ofstream* nodeLog = NULL;
	
		if( m_nodeLogs.find( m_currentNode ) == m_nodeLogs.end() )
		{
			std::stringstream in;
			in << m_delegate.Name() << ".node." << m_currentNode << ".log";
			nodeLog = new std::ofstream( in.str().c_str(), std::ofstream::trunc );
			m_nodeLogs[ m_currentNode ] = nodeLog;
		}
		else
		{
			nodeLog = dynamic_cast<std::ofstream*>( m_nodeLogs[ m_currentNode ] );
		}
		
		return *nodeLog;
	}
	
	RDTimeStamp Logger::CurrentTime() const
	{
		return m_delegate.Time();
	}
	
	void Logger::Initialize( SDRPDelegate& delegate )
	{
		if( Logger::m_instance == NULL )
		{
			Logger::m_instance = new Logger( delegate );
		}	
	}

	Logger* Logger::Instance()
	{		
		return Logger::m_instance;
	}
} }
 
