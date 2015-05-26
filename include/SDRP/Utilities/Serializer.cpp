/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/

#include <SDRP/Utilities/Serializer.h>

namespace Radicle { namespace SDRP
{
	bool Serializer::ShouldSwap = Serializer::LittleEndian();

	bool Serializer::LittleEndian()
	{
		RDUInt16 test = 0x4321;
		return( *( ( RDUByte8* ) &test ) == 0x21 );
	}
	
	bool Serializer::EndianSwap( void* buffer, const RDSize bufferSize )
	{
		RDSize i = 0, j = bufferSize - 1;
		RDUByte8 temp;
		RDUByte8* byteBuffer = reinterpret_cast<RDUByte8*>( buffer );
		
		while( i < j )
		{
			temp 		= byteBuffer[i];
			byteBuffer[i] 	= byteBuffer[j];
			byteBuffer[j] 	= temp;
			++i;
			--j;
		}

		return true;
	}
	
	bool Serializer::Serialize( 	RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, const RDUInt16 integer )
	{
		return 	BufferPack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( buffer + offset, sizeof( integer ) );
	}
	bool Serializer::Serialize( 	RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, const RDInt16 integer )
	{
		return 	BufferPack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( buffer + offset, sizeof( integer ) );
	}
	bool Serializer::Serialize( 	RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, const RDUInt32 integer )
	{
		return 	BufferPack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( buffer + offset, sizeof( integer ) );
	}
	bool Serializer::Serialize( 	RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, const RDInt32 integer )
	{
		return 	BufferPack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( buffer + offset, sizeof( integer ) );
	}
	bool Serializer::Serialize( 	RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, const RDSize integer )
	{
		return 	BufferPack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( buffer + offset, sizeof( integer ) );
	}
	
	bool Serializer::Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDUInt16& integer )
	{
		return 	BufferUnpack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( &integer, sizeof( integer ) );
	}
	bool Serializer::Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDInt16& integer )
	{
		return 	BufferUnpack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( &integer, sizeof( integer ) );
	}
	bool Serializer::Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDUInt32& integer )
	{
		return 	BufferUnpack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( &integer, sizeof( integer ) );
	}
	bool Serializer::Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDInt32& integer )
	{
		return 	BufferUnpack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( &integer, sizeof( integer ) );
	}
	bool Serializer::Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDSize& integer )
	{
		return 	BufferUnpack( buffer, bufferSize, offset, newOffset, &integer, sizeof( integer ) ) &&
			EndianSwap( &integer, sizeof( integer ) );
	}
	
	bool Serializer::BufferPack(	RDUByte8* buffer,
					const RDSize bufferSize,
					const RDSize offset,
					RDSize& newOffset,
					const void* object,
					const RDSize objectSize )
	{
		if( bufferSize - offset >= objectSize  )
		{
			const RDUByte8* objectData = reinterpret_cast<const RDUByte8*>( object );
			for( RDSize i = 0; i < objectSize; ++i )
			{
				*( buffer + offset + i ) = *( objectData + i );
			}
			newOffset = offset + objectSize;
			
			return true;
		}
		
		return false;
	}
	
	bool Serializer::BufferUnpack(	const RDUByte8* buffer,
					const RDSize bufferSize,
					const RDSize offset,
					RDSize& newOffset,
					void* object,
					const RDSize objectSize )
	{
		if( bufferSize - offset >= objectSize  )
		{
			RDUByte8* objectData = reinterpret_cast<RDUByte8*>( object );
			for( RDSize i = 0; i < objectSize; ++i )
			{
				*( objectData + i ) = *( buffer + offset + i );
			}
			newOffset = offset + objectSize;
			
			return true;
		}
		
		return false;
	}
	
	RDUInt16 Serializer::Checksum( 	const RDUByte8* buffer,
					const RDSize bufferSize )
	{
		RDUInt16 chunk, i;
		RDUInt32 sum = 0;

		for( i=0; i < bufferSize; i = i + 2 )
		{
			chunk = ( ( buffer[i] << 8 ) & 0xFF00 ) + ( buffer[i+1] & 0xFF );
			sum = sum + static_cast<RDUInt32>( chunk );	
		}

		while( sum >> 16 )
		{
			sum = ( sum & 0xFFFF ) + ( sum >> 16 );
		}

		sum = ~sum;
	
		return static_cast<RDUInt16>( sum );
	}
} }
