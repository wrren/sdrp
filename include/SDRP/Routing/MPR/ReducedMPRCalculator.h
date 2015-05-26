/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2012 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_REDUCED_MPR_CALCULATOR_H
#define RD_SDRP_REDUCED_MPR_CALCULATOR_H

#include <SDRP/Routing/MPR/IMPRCalculator.h>

namespace Radicle { namespace SDRP {
	/**
	 *	Calculator for MPRs based on differently-size bloom filters
	 */
	class ReducedMPRCalculator : public IMPRCalculator
	{
	public:

		/**
		 *	Default Constructor
		 */
		ReducedMPRCalculator( const Node& node ) : IMPRCalculator( node ) {}

		/**
		 *	Calculate a set of MPR Nodes using the given neighbour information
		 * @param neighbours 	Current Node Neighbours
		 * @return 		Bloom Filter Describing MPR Nodes
		 */
		virtual BloomFilter Calculate( const std::set<Node>& neighbours );

	protected:

		/**
		 *	Class used to contain RMPR data for MPR selection
		 */
		class RMPRContainer
		{
		public:

			/**
			 *	Default Constructor
			 * @param node 		Node
			 * @param difference 	Degree of difference between node's neighbours and mine
			 */
			RMPRContainer( const Node& node, const float difference = 0.0F );

			/**
			 *	Set the node
			 * @param node 	
			 */
			void Node( const Radicle::SDRP::Node& node  );
			
			/**
			 *	Get the node
			 * @return node
			 */
			const Radicle::SDRP::Node& Node() const;

			/**
			 *	Set the difference
			 * @param difference 	
			 */
			void Difference( float difference  );
			
			/**
			 *	Get the difference
			 * @return difference
			 */
			float Difference() const;

			/**
			 *	Comparison Operators
			 */
			bool operator<( const RMPRContainer& other ) const;
			bool operator>( const RMPRContainer & other ) const;
			bool operator==( const RMPRContainer& other ) const;
			bool operator!=( const RMPRContainer& other ) const;

		protected:

			/// Neighbour Node
			Radicle::SDRP::Node		m_node;
			/// Difference between node's neighbours and mine
			float 		m_difference;
		};

		/**
		 *	Iterate over the node container and find nodes with matching neighbour filter sizes, then
		 *	compare them in terms of the differences between the filters, eliminating nodes with
		 *	filter which don't have significant differences.
		 * @param container 		Node Container
		 * @param minimumDifference 	Minimum difference expressed as a floating point number quantified as
		 *				the ratio of the number of differently set bits in the two filters to
		 *				the size of the filters being compared.
		 */
		static NodeContainer EliminateSimilarNodes( const NodeContainer& container, const float minimumDifference );

		static float Mean( const std::set<RMPRContainer>& differenceSet );

		static float StdDev( const std::set<RMPRContainer>& differenceSet );
	};
}}

#endif // RD_SDRP_REDUCED_MPR_CALCULATOR_H