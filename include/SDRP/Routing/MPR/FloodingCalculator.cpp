/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#include <SDRP/Routing/MPR/FloodingCalculator.h>

namespace Radicle { namespace SDRP {
	
	BloomFilter FloodingCalculator::Calculate( const std::set<Node>& neighbours )
	{
		return BloomFilter( ( RDSize ) 1, ( RDSize ) 0 ).Universe();
	}
}}