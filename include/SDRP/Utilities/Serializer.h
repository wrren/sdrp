/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_SERIALIZER_H
#define RD_SDRP_SERIALIZER_H

#include <SDRP/Core/Types.h>

namespace Radicle { namespace SDRP
{
	/**
	 *	Serializer is a utility class used for binary serialization and deserialization of 
	 *	primitive types and ISerializable objects
	 */
	class Serializer
	{
	public:
		
		/**
		 *	Serialize the provided integer into the target buffer, performing an
		 *	endian-swap if necessary.
		 * @param buffer	Data buffer into which the integer should be serialized
		 * @param bufferSize	Size of the buffer in bytes
		 * @param offset	Offset into the buffer at which serialization should begin
		 * @param newOffset	New offset produced by serializing the integer
		 * @param integer	Integer to be serialized
		 * @return		True - If serialization was successful. False otherwise.
		 */
		static bool Serialize( RDUByte8* buffer, const RDSize bufferSize, const RDSize offset, RDSize& newOffset, const RDUInt16 integer );
		static bool Serialize( RDUByte8* buffer, const RDSize bufferSize, const RDSize offset, RDSize& newOffset, const RDInt16 integer );
		static bool Serialize( RDUByte8* buffer, const RDSize bufferSize, const RDSize offset, RDSize& newOffset, const RDUInt32 integer );
		static bool Serialize( RDUByte8* buffer, const RDSize bufferSize, const RDSize offset, RDSize& newOffset, const RDInt32 integer );
		static bool Serialize( RDUByte8* buffer, const RDSize bufferSize, const RDSize offset, RDSize& newOffset, const RDSize integer );
		
		/**
		 *	Deserialize an integer from the given buffer
		 * @param buffer	Data buffer from which the integer should be deserialized
		 * @param bufferSize	Size of the buffer in bytes
		 * @param offset	Offset into the buffer at which deserialization should begin
		 * @param newOffset	New offset produced by deserializing the integer
		 * @param integer	Integer to be deserialized
		 * @return		True - If deserialization was successful. False otherwise.
		 */
		static bool Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDUInt16& integer );
		static bool Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDInt16& integer );
		static bool Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDUInt32& integer );
		static bool Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDInt32& integer );
		static bool Deserialize( const RDUByte8* buffer, const RDSize bufferSize, const RDSize offset,	RDSize& newOffset, RDSize& integer );
		
		/**
		 *	Serialize the provided object into the target buffer
		 * @param buffer	Data buffer into which the object should be serialized
		 * @param bufferSize	Size of the buffer in bytes
		 * @param offset	Offset into the buffer at which serialization should begin
		 * @param newOffset	New offset produced by serializing the object
		 * @param object	Object to be serialized
		 * @return		True - If serialization was successful. False otherwise.
		 */
		template< typename T >
		static bool Serialize( 		RDUByte8* buffer,
		 				const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset,
						const T& object )
		{
			return BufferPack( buffer, bufferSize, offset, newOffset, &object, sizeof( T ) );
		}
					
		
		/**
		 *	Deserialize an object from the given buffer
		 * @param buffer	Data buffer from which the object should be deserialized
		 * @param bufferSize	Size of the buffer in bytes
		 * @param offset	Offset into the buffer at which deserialization should begin
		 * @param newOffset	New offset produced by deserializing the object
		 * @param object	Object to be deserialized
		 * @return		True - If deserialization was successful. False otherwise.
		 */
		template< typename T >
		static bool Deserialize( 	const RDUByte8* buffer,
		 				const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset,
						T& object )
		{
			return BufferUnpack( buffer, bufferSize, offset, newOffset, &object, sizeof( T ) );
		}
	
		/**
		 *	Pack the provided object into the given buffer
		 * @param buffer	Data buffer into which the object should be packed	
		 * @param bufferSize	Size of the buffer in bytes
		 * @param offset	Offset into the buffer at which packing should begin
		 * @param newOffset	New offset produced by packing the object
		 * @param object	Object to be packed
		 * @param objectSize	Size of the object
		 * @return		True - If serialization was successful. False otherwise.
		 */
		static bool BufferPack(		RDUByte8* buffer,
						const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset,
						const void* object,
						const RDSize objectSize );
		
		/**
		 *	Unpack the provided object from the given buffer
		 * @param buffer	Data buffer from which the object should be unpacked
		 * @param bufferSize	Size of the buffer in bytes
		 * @param offset	Offset into the buffer at which unpacking should begin
		 * @param newOffset	New offset produced by unpacking the object
		 * @param object	Object to be unpacked
		 * @return		True - If unpacking was successful. False otherwise.
		 */				
		static bool BufferUnpack(	const RDUByte8* buffer,
						const RDSize bufferSize,
						const RDSize offset,
						RDSize& newOffset,
						void* object,
						const RDSize objectSize );
		
		
		/**
		 *	Generate a checksum for the provided data
		 * @param buffer	Data Buffer
		 * @param bufferSizeq	Buffer Size in Bytes
		 * @return		Buffer Checksum
		 */
		static RDUInt16 Checksum(	const RDUByte8* buffer,
						const RDSize bufferSize );
	
	private:					
		
		/// Indicates whether Endian Swapping should occur. Calculated at runtime.
		static bool ShouldSwap;
		
		/**
		 *	Runtime check for little-endianness
		 * @return		True - If system is little-endian. False otherwise.
		 */
		static bool LittleEndian();
		
		/**
		 *	Perform an endian swap on the provided buffer content
		 * @param buffer	Data buffer
		 * @param bufferSize	Buffer size or number of bytes to swap
		 * @returnq		True
		 */
		static bool EndianSwap( 	void* buffer, const RDSize bufferSize );		
	};
} }

#endif // RD_SDRP_SERIALIZER_H

