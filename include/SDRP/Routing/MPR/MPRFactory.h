/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_MPR_FACTORY_H
#define RD_SDRP_MPR_FACTORY_H


#include <SDRP/Core/Core.h>
#include <SDRP/Routing/MPR/IMPRCalculator.h>

namespace Radicle { namespace SDRP {

	/**
	 *	Factory class for MPR Calculators
	 */
	class MPRFactory
	{
	public:

		/// MPR Calculator Auto-Pointer Type
		typedef std::auto_ptr<IMPRCalculator> 	MPRCalculatorPtr;

		/// Possible MPR Selection Modes
		enum MPRSelectionMode
		{
			Flooding,
			MPR,
			ReducedMPR
		};

		MPRFactory( const Node& localNode ) :
		m_node( localNode )
		{}

		/**
		 *	Get an MPR Calculator for the given selection mode
		 * @param mode 		MPR Selection Mode
		 */
		MPRCalculatorPtr GetCalculator( const MPRSelectionMode mode );

	protected:

		Node m_node;
	};
}}

#endif // RD_SDRP_MPR_FACTORY_H