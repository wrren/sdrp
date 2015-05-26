/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#include <SDRP/Routing/MPR/MPRFactory.h>

#include <SDRP/Routing/MPR/MPRCalculator.h>
#include <SDRP/Routing/MPR/FloodingCalculator.h>
#include <SDRP/Routing/MPR/ReducedMPRCalculator.h>

namespace Radicle { namespace SDRP {

	MPRFactory::MPRCalculatorPtr MPRFactory::GetCalculator( const MPRFactory::MPRSelectionMode mode )
	{
		MPRCalculatorPtr calculator;

		if( mode == MPRFactory::Flooding )
		{
			calculator.reset( new FloodingCalculator( m_node ) );
		}
		else if( mode == MPRFactory::ReducedMPR )
		{
			calculator.reset( new ReducedMPRCalculator( m_node ) );
		}
		else
		{
			calculator.reset( new MPRCalculator( m_node ) );
		}

		return calculator;
	}
}}