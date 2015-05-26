/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Core/BloomFilter.h>
#include <SDRP/Core/Macros.h>
#include <SDRP/Core/ErrorCodes.h>
#include <SDRP/Utilities/Serializer.h>
#include <SDRP/Utilities/MurmurHash.h>
#include <limits>
#include <complex>
#include <cstring>

namespace Radicle { namespace SDRP
{
	const RDSize		BloomFilter::BitsPerChar		= 8;
	RDSize 			BloomFilter::PredictedElementCount 	= 30;
	RDDouble		BloomFilter::DesiredFalsePositiveRate 	= 0.1;
	const BloomFilter	BloomFilter::Empty( ( RDSize ) 0, ( RDSize ) 0 );

	std::map< std::pair<RDSize, RDDouble>, std::pair<RDSize, RDSize> > BloomFilter::m_cachedParameters;

	BloomFilter::BloomFilter( 	const RDSize numElements, 
					const RDDouble falsePositiveRate ) :
	m_tableSize( 0 ),
	m_hashCount( 0 ),
	m_table( NULL )
	{		
		RDSize tableSize, hashCount;
		CalculateParameters( numElements, falsePositiveRate >= 1.0 ? 0.1 : falsePositiveRate , tableSize, hashCount );
		Initialize( tableSize, hashCount );
	}
	
	BloomFilter::BloomFilter( const RDSize tableSize, const RDSize hashCount ) :
	m_table( NULL ), m_tableSize( 0 ), m_hashCount( 0 )
	{
		Initialize( tableSize, hashCount );
	}
	
	BloomFilter::BloomFilter( const BloomFilter& other ) :
	m_table( NULL ), m_tableSize( 0 ), m_hashCount( 0 )
	{
		( *this ) = other;
	}

	void BloomFilter::ResetWithParameters( 	const RDSize numElements, 
						const RDDouble falsePositiveRate )
	{
		CalculateParameters( numElements, falsePositiveRate >= 1.0 ? 0.1 : falsePositiveRate , m_tableSize, m_hashCount );
		Initialize( m_tableSize, m_hashCount );
	}

	BloomFilter& BloomFilter::Insert( const RDIdentifier id )
	{
		if( m_tableSize > 0 )
		{
			RDUInt32 seed = 0;
		
			for( RDUInt32 i = 0; i < m_hashCount; i++ )
			{
				seed = MurmurHash::Hash( id, seed );
				Set( seed );
			}
		}

		return ( *this );
	}

	bool BloomFilter::Contains( const RDIdentifier id ) const
	{
		if( m_tableSize > 0 )
		{
			RDUInt32 seed = 0;
		
			for( RDUInt32 i = 0; i < m_hashCount; i++ )
			{
				seed = MurmurHash::Hash( id, seed );
				if( Check( seed ) == false )
				{
					return false;
				}
			}
		
			return true;
		}
		
		return false;
	}
	
	bool BloomFilter::Contains( const BloomFilter& filter ) const
	{
		if( m_tableSize > 0 )
		{
			for( RDSize i = 0; i < m_tableSize; ++i )
			{
				if( filter.Check( i ) )
				{
					if( Check( i ) == false )
					{
						return false;
					}
				}
			}
		
			return true;
		}
		
		return false;
	}
	
	BloomFilter& BloomFilter::Remove( const RDIdentifier id )
	{
		if( m_tableSize > 0 && Contains( id ) )
		{
			RDUInt32 seed = 0;
		
			for( RDUInt32 i = 0; i < m_hashCount; i++ )
			{
				seed = MurmurHash::Hash( id, seed ) % m_tableSize;
			
				if( m_table[ seed ] > 0 )
				{
					m_table[ seed ]--;
				}
			}
		}

		return ( *this );
	}
	
	BloomFilter& BloomFilter::Remove( const BloomFilter& other ) throw( BloomFilterSizeMismatchException )
	{
		if( m_tableSize != other.m_tableSize )
		{
			throw BloomFilterSizeMismatchException();
		}

		if( m_tableSize > 0 )
		{
			if( this != &other )
			{
				for( RDSize i = 0; i < m_tableSize; ++i )
				{
					if( m_table[i] > other.m_table[i] )
					{
						m_table[i] -= other.m_table[i];
					}
					else
					{
						m_table[i] = 0;
					}
				}
			}
			else
			{
				Clear();
			}
		}

		return ( *this );
	}
	
	BloomFilter& BloomFilter::Universe()
	{
		for( RDSize i = 0; i < m_tableSize; i++ )
		{
			m_table[i] = 1;
		}
		return ( *this );
	}
	
	BloomFilter& BloomFilter::DestructiveResize( const RDSize tableSize, const RDSize hashCount )
	{
		Initialize( tableSize, hashCount );
		return ( *this );
	}

	bool BloomFilter::RemovalResultsInDifference( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException )
	{
		if( m_tableSize != other.m_tableSize )
		{
			throw BloomFilterSizeMismatchException();
		}

		for( RDSize i = 0; i < m_tableSize; ++i )
		{
			if( m_table[i] > 0 && m_table[i] <= other.m_table[i] )
			{
				return true;
			}
		}

	
		return false;
	}

	bool BloomFilter::HasElements() const
	{
		for( RDSize i = 0; i < m_tableSize; i++ )
		{
			if( m_table[i] != 0 )
			{
				return true;
			}
		}
		
		return false;
	}
	
	const RDSize BloomFilter::TableSize() const
	{
		return m_tableSize;
	}

	const RDSize BloomFilter::HashCount() const
	{
		return m_hashCount;
	}
	
	const RDSize BloomFilter::SetBytes() const
	{
		RDSize set = 0;
		
		for( RDSize i = 0; i < m_tableSize; i++ )
		{
			if( Check( i ) )
			{
				set++;
			}
		}
		
		return set;
	}
		
	BloomFilter& BloomFilter::Clear()
	{
		std::fill_n( m_table, m_tableSize, static_cast<RDUByte8>( 0x00 ) );
		return ( *this );
	}

	bool BloomFilter::Serialize( 	RDUByte8* buffer,
					const RDSize bufferSize,
					const RDSize offset,
					RDSize& newOffset ) const
	{
		if( 	Serializer::Serialize( buffer, bufferSize, offset, newOffset, m_tableSize ) &&
			Serializer::Serialize( buffer, bufferSize, newOffset, newOffset, m_hashCount ) )
		{
			if( m_tableSize > 0 )
			{
				RDSize bitBufferSize = m_tableSize < BitsPerChar ? BitsPerChar : m_tableSize / BitsPerChar;
				RDUByte8* bitBuffer = new RDUByte8[ bitBufferSize ];
				std::fill_n( bitBuffer, bitBufferSize, static_cast<RDUByte8>( 0x00 ) );
				bool success = false;
		
				for( RDSize bit = 0; bit < m_tableSize; ++bit )
				{
					if( Check( bit ) )
					{
						SetBit( bitBuffer, bit );
					}
				}
		
				if( Serializer::BufferPack( buffer, bufferSize, newOffset, newOffset, bitBuffer, bitBufferSize ) )
				{
					success = true;
				}
		
				delete[] bitBuffer;
				
				return success;
			}
			else
			{
				return true;
			}
		}
		
		return false;
	}

	bool BloomFilter::Deserialize( 	const RDUByte8* buffer,
	 				const RDSize bufferSize,
					const RDSize offset,
					RDSize& newOffset )
	{
		RDSize tableSize, hashCount;

		if( 	Serializer::Deserialize( buffer, bufferSize, offset, newOffset, tableSize ) &&
			Serializer::Deserialize( buffer, bufferSize, newOffset, newOffset, hashCount ) )
		{			
			Initialize( tableSize, hashCount );
			
			if( m_tableSize > 0 )
			{
				bool success = false;

				RDSize bitBufferSize = m_tableSize < BitsPerChar ? BitsPerChar : m_tableSize / BitsPerChar;
				RDUByte8* bitBuffer = new RDUByte8[ bitBufferSize ];
			
				if( Serializer::BufferUnpack( buffer, bufferSize, newOffset, newOffset, bitBuffer, bitBufferSize ) )
				{
					for( RDSize i = 0; i < m_tableSize; i++ )
					{
						if( CheckBit( bitBuffer, i ) )
						{
							Set( i );
						}
					}
				
					success = true;
				}
				else
				{
					RD_PRINT( "Failed to Deserialize Table Buffer" );
				}

			
				delete[] bitBuffer;
			
				return success;
			}
			else
			{
				return true;
			}
		}
		else
		{
			RD_PRINT( "Failed to Deserialize Table Size and Hash Count" );
		}
		
		return false;
	}
	
	BloomFilter BloomFilter::Intersection( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException )
	{
		if( m_tableSize != other.m_tableSize )
		{
			throw BloomFilterSizeMismatchException();
		}

		BloomFilter intersection( other.m_tableSize, other.m_hashCount );

		for( RDSize i = 0; i < m_tableSize; i++ )
		{
			intersection.m_table[i] = Check( i ) == true && other.Check( i ) == true ? 1 : 0;
		}

		return intersection;
	}

	BloomFilter BloomFilter::Union( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException )
	{
		if( m_tableSize != other.m_tableSize )
		{
			throw BloomFilterSizeMismatchException();
		}

		BloomFilter filterUnion( other.m_tableSize, other.m_hashCount );

		for( RDSize i = 0; i < m_tableSize; i++ )
		{
			filterUnion.m_table[i] = m_table[i] + other.m_table[i];
		}

		return filterUnion;
	}
	
	RDSize BloomFilter::Difference( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException )
	{
		if( m_tableSize != other.m_tableSize )
		{
			throw BloomFilterSizeMismatchException();
		}

		RDSize difference = 0;

		for( RDSize i = 0; i < m_tableSize; i++ )
		{
			if( Check( i ) != other.Check( i ) )
			{
				difference++;
			}
		}
		
		return difference;
	}

	RDSize BloomFilter::SetDifference( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException )
	{
		if( m_tableSize != other.m_tableSize )
		{
			throw BloomFilterSizeMismatchException();
		}

		RDSize difference = 0;

		for( RDSize i = 0; i < m_tableSize; i++ )
		{
			if( Check( i ) != other.Check( i ) )
			{
				difference++;
			}
		}
		
		return difference;
	}
	
	void BloomFilter::Print( std::ostream& out ) const
	{
		for( RDSize i = 0; i < m_tableSize; i++ )
		{
			if( Check( i ) )
			{
				out << "1";	
			}
			else
			{
				out << "0";
			}
		}
		
		out << std::endl;
	}
	
	void BloomFilter::CalculateParameters( 	const RDSize numElements, 
						const RDDouble falsePositiveRate, 
						RDSize& tableSize,
						RDSize& hashCount )
	{
		std::pair<RDSize, RDDouble> parameters( numElements, falsePositiveRate );
		
		if( m_cachedParameters.find( parameters ) == m_cachedParameters.end() )
		{
	      		tableSize	= static_cast<int>(  -( numElements * std::log( falsePositiveRate ) ) / std::pow( std::log( 2.0 ), 2 ) );
			hashCount	= ( tableSize / numElements ) * std::log( 2.0 );
	      		
	      		std::pair<RDSize, RDSize> result( hashCount, tableSize );
	      		
	      		m_cachedParameters[ parameters ] = result;
      		}
      		else
      		{
      			std::pair<RDSize, RDSize> result = m_cachedParameters[ parameters ];
      			
      			hashCount = result.first;
      			tableSize = result.second;
      		}
      		
      		tableSize = tableSize % BitsPerChar == 0 ? tableSize : tableSize + ( BitsPerChar - tableSize % BitsPerChar );
	}
	
	void BloomFilter::Initialize( const RDSize tableSize, const RDSize hashCount )
	{	
		m_hashCount = hashCount;
		m_tableSize = tableSize;

		if( m_table != NULL )
		{
			delete[] m_table;
			m_table = NULL;	
		}
	
		if( m_tableSize > 0 )
		{		
			m_table = new RDUByte8[ static_cast<RDUInt32>( m_tableSize ) ];
		}

		if( m_tableSize > 0 )
		{
			std::fill_n( m_table, m_tableSize, static_cast<RDUByte8>( 0x00 ) );
		}
	}
	
	BloomFilter& BloomFilter::Set( const RDSize index )
	{
		m_table[ index % m_tableSize ]++;
		return ( *this );
	}

	bool BloomFilter::Check( const RDSize index ) const
	{
		return m_table[ index % m_tableSize ] != 0;
	}
	
	bool BloomFilter::CheckBit( const RDUByte8* buffer, const RDSize bit ) const
	{
		static const RDUByte8 bitMask[ BitsPerChar ] = {	0x01,  //00000001
									0x02,  //00000010
									0x04,  //00000100
									0x08,  //00001000
									0x10,  //00010000
									0x20,  //00100000
									0x40,  //01000000
									0x80   //10000000
									};
										
		RDSize byteIndex	= bit / BitsPerChar;
		RDSize bitIndex 	= bit % BitsPerChar;
		
		return ( buffer[ byteIndex ] & bitMask[ bitIndex ] ) == bitMask[ bitIndex ];
	}
	
	void BloomFilter::SetBit( RDUByte8* buffer, const RDSize bit ) const
	{
		static const RDUByte8 bitMask[ BitsPerChar ] = {	0x01,  //00000001
									0x02,  //00000010
									0x04,  //00000100
									0x08,  //00001000
									0x10,  //00010000
									0x20,  //00100000
									0x40,  //01000000
									0x80   //10000000
									};
										
		RDSize byteIndex	= bit / BitsPerChar;
		RDSize bitIndex 	= bit % BitsPerChar;
		
		buffer[ byteIndex ] |= bitMask[ bitIndex ];
	}
	
	bool BloomFilter::operator==( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException )
	{
		if( m_tableSize != other.m_tableSize )
		{
			throw BloomFilterSizeMismatchException();
		}

		return Difference( other ) == 0;
	}

	bool BloomFilter::operator!=( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException )
	{
		return !( ( *this ) == other );
	}
	
	bool BloomFilter::operator>( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException )
	{
		if( m_tableSize != other.m_tableSize )
		{
			throw BloomFilterSizeMismatchException();
		}

		return SetBytes() > other.SetBytes();
	}
	
	bool BloomFilter::operator<( const BloomFilter& other ) const throw( BloomFilterSizeMismatchException )
	{
		if( m_tableSize != other.m_tableSize )
		{
			throw BloomFilterSizeMismatchException();
		}

		return !( ( *this ) > other );
	}

	BloomFilter& BloomFilter::operator=( const BloomFilter& other )
	{
		if( this != &other )
		{
			Initialize( other.m_tableSize, other.m_hashCount );
			memcpy( m_table, other.m_table, m_tableSize );
		}
	}
	
	BloomFilter::~BloomFilter()
	{	
		if( m_table != NULL )
		{
			delete[] m_table;
		}
	}
} }

