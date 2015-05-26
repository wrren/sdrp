/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_BLOOM_FILTER_H
#define RD_SDRP_BLOOM_FILTER_H

#include <map>
#include <iostream>
#include <SDRP/Core/Types.h>
#include <SDRP/Core/Exception.h>
#include <SDRP/Core/ISerializable.h>

namespace Radicle { namespace SDRP
{
	/**
	 *	Bloom Filter Size Mismatch Exception
	 */
	class BloomFilterSizeMismatchException : public Exception
	{
	public:

		/**
		 *	Get the message associated with this exception
		 * @return 	Exception Message
		 */
		virtual const char* Message() const throw()
		{
			return "Bloom Filter Size Mismatch!";
		}
	};

	/**
	 *	A Bloom Filter is a static-length set representation which may
	 *	be used to check for the presence of data but which may not retrieve
	 *	that data.
	 */
	class BloomFilter : public ISerializable
	{
	public:
		
		/// Number of Bits in a Char Type
		const static RDSize		BitsPerChar;
		/// Predicted Number of Contained Elements
		static RDSize 			PredictedElementCount;
		/// Desired False Positive Rate
		static RDDouble			DesiredFalsePositiveRate;
		/// Simple Empty Bloom Filter
		static const BloomFilter	Empty;
				
		/**
		 *	Default Constructor
		 * @param numElements		Number of Elements to be Contained
		 * @param falsePositiveRate	Maximum False Positive Rate
		 */
		BloomFilter( 	const RDSize numElements = PredictedElementCount, 
				const RDDouble falsePositiveRate = DesiredFalsePositiveRate );
				
		/**
		 *	Initializing Constructor
		 * @param tableSize	Table Size in Bits
		 * @param hashCount	Number of hashes to use per insertion
		 */
		BloomFilter(	const RDSize tableSize, const RDSize hashCount );
		
		/**
		 *	Copy Constructor
		 * @param other	Other Bloom Filter
		 */
		BloomFilter(	const BloomFilter& other );

		/**
		 *	Reset the bloom filter with new parameters
		 * @param numElements		Number of Elements to be Contained
		 * @param falsePositiveRate	Maximum False Positive Rate
		 */
		void ResetWithParameters( 	const RDSize numElements = PredictedElementCount, 
						const RDDouble falsePositiveRate = DesiredFalsePositiveRate );
		
		/**
		 *	Insert the provided identifier into the bloom filter
		 * @param id	Identifier to be inserted
		 */
		BloomFilter& Insert( const RDIdentifier id );
		
		/**
		 *	Check whether the bloom filter contains the given identifier
		 * @param id	Identifier to be checked
		 * @return	True - If the bloom filter contains the identifier. False otherwise.
		 */
		bool Contains( const RDIdentifier id ) const;
		
		/**
		 *	Check whether the bloom filter contains all of the elements in the given filter
		 * @param filter	Filter
		 * @return	True - If this filter contains all of the elements in \a filter. False otherwise.
		 */
		bool Contains( const BloomFilter& filter ) const;
		
		/**
		 *	Remove the provided identifer from the bloom filter
		 * @param id	Identifier to be removed
		 */
		BloomFilter& Remove( const RDIdentifier id );
		
		/**
		 *	Remove the specified bloom filter contents from this filter
		 * @param other	Filter to be removed
		 */
		BloomFilter& Remove( const BloomFilter& other ) throw( BloomFilterSizeMismatchException );
		
		/**
		 *	Completely fill the bloom filter so that all checks against its content will return true
		 */
		BloomFilter& Universe();
		
		/**
		 *	Destructively resize the bloom filter
		 * @param tableSize	New Table Size
		 * @param hashCount	New Hash Count
		 */
		BloomFilter& DestructiveResize( const RDSize tableSize, const RDSize hashCount );
		
		/**
		 *	Check whether the removal of the elements contained in \a other would result in a change
		 *	to this filter
		 * @param other	The other bloom filter
		 * @return	True - If the removal results in a difference. False otherwise.
		 */
		bool RemovalResultsInDifference( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException );
		
		/**
		 *	Check whether this bloom filter contains any elements
		 * @return	True - If the bloom filter contains any elements. False otherwise.
		 */
		bool HasElements() const;
		
		/**
		 *	Get the size of the filter in bytes
		 * @return	Size of the filter in bytes
		 */
		const RDSize TableSize() const;

		/**
		 *	Get the number of hashes used for filter insertions
		 * @return 	Hash count
		 */
		const RDSize HashCount() const;
		
		/**
		 *	Clear all elements from the bloom filter
		 */
		BloomFilter& Clear();
		
		/**
		 *	Get the number of bytes set in this bloom filter
		 * @return	Number of bytes set in this bloom filter
		 */
		const RDSize SetBytes() const;
		
		/**
		 *	Get the intersection between this bloom filter and another
		 * @param other	Other Bloom Filter
		 * @return	Intersection between this bloom filter and other
		 */
		BloomFilter Intersection( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException );
		
		/**
		 *	Get the union of this bloom filter and another
		 * @param other	Other Bloom Filter
		 * @return	Union of this bloom filter and other
		 */
		BloomFilter Union( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException );
		
		/**
		 *	Get the degree of difference between this bloom filter and another
		 * @param other	Other Bloom Filter
		 * @return	Degree of difference in terms of the number of bits differently set between
		 *		this filter and other
		 */
		RDSize Difference( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException );

		/**
		 *	Get the degree of difference between this bloom filter and another in terms of the number 
		 *	of bits set in other that are not set in this filter.
		 * @param other	Other Bloom Filter
		 * @return	Degree of difference in terms of the number 
		 *		of bits set in other that are not set in this filter.
		 */
		RDSize SetDifference( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException );
		
		/**
		 *	Print the filter to the specified output stream
		 * @param out	Output Stream
		 */
		void Print( std::ostream& out ) const;
		
		/**
		 *	Check whether the specified table index is set
		 * @return 	True - If the specified table index is set. False otherwise
		 */
		bool Check( const RDSize index ) const;
		
		/**
		 *	Set or increment the specified table index
		 * @param index	Index to be set or incremented
		 */
		BloomFilter& Set( const RDSize index );
		
		/**
		 *	Serialize this object into the provided data buffer.
		 * @param buffer	Data buffer into which the object should be serialized
		 * @param bufferSize	Size of the data buffer in bytes
		 * @param offset	Offset into the buffer at which serialization should begin
		 * @param newOffset	New offset produced by serialization
		 * @return		True - If serialization was successful. False otherwise.
		 */
		virtual bool Serialize( 	RDUByte8* buffer,
						const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset ) const;
			
		/**
		 *	Deserialize the this object from the provided data buffer
		 * @param buffer	Data buffer from which the object should be deserialized
		 * @param bufferSize	Size of the buffer in bytes
		 * @param offset	Offset into the buffer at which deserialization should begin
		 * @param newOffset	New offset produced by deserializing the object
		 * @return		True - If deserialization was successful. False otherwise.
		 */
		virtual bool Deserialize( 	const RDUByte8* buffer,
		 				const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset );
		
		/**
		 *	Comparison Operators
		 */	
		bool operator==( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException );
		bool operator!=( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException );;
		bool operator>( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException );
		bool operator<( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException );
		
		/**
		 *	Assignment Operator
		 */
		BloomFilter& operator=( const BloomFilter& other );
		
		/**
		 *	Default Destructor
		 */
		~BloomFilter();
		
	private:
	
		/// Cached Calculated Parameters
		static std::map< std::pair<RDSize, RDDouble>, std::pair<RDSize, RDSize> > m_cachedParameters;
	
		/**
		 *	Calculate Filter Parameters based on Inputs
		 * @param numElements		Number of elements to be contained in the filter
		 * @param falsePositiveRate	Maximum allowable false positive rate
		 * @param tableSize[out]	Outputted table size in bytes
		 * @param hashCount[out]	Outputted number of hashes to use for insertions
		 */
		static void CalculateParameters( 	const RDSize numElements, 
							const RDDouble falsePositiveRate,
							RDSize& tableSize,
							RDSize& hashCount );
		
		/**
		 *	Initialize the Bloom Filter with the provided parameters
		 * @param tableSize	Size of the Data Table in Bytes
		 * @param hasCount	Number of hashes to use on insertion
		 */
		void Initialize( const RDSize tableSize, const RDSize hashCount );
		
		/**
		 *	Check whether the bit at the specified index in the provided buffer is set
		 * @param buffer	Bit Buffer
		 * @param bit		Bit Index to be Checked
		 * @return		True - If the bit index is set. False otherwise.
		 */
		bool CheckBit( const RDUByte8* buffer, const RDSize bit ) const;
		
		/**
		 *	Set the bit at the specified index in the provided buffer
		 * @param buffer	Bit Buffer
		 * @param bit		Bit Index to be Set
		 */
		void SetBit( RDUByte8* buffer, const RDSize bit ) const;
			
		/// Data Table
		RDUByte8*	m_table;
		/// Size of the Bit Table in Bytes
		RDSize		m_tableSize;	
		/// Number of Hashes per Element
		RDSize		m_hashCount;		
	};
} }

#endif // RD_SDRP_BLOOM_FILTER_H
 
