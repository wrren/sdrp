/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_ISERIALIZABLE_H
#define RD_SDRP_ISERIALIZABLE_H

#include <SDRP/Core/Types.h>
#include <SDRP/Utilities/Serializer.h>

namespace Radicle { namespace SDRP
{
	/** 
	 *	Common interface for serializable classes.
	 */
	class ISerializable
	{
	public:
		
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
						RDSize& newOffset ) const = 0;
			
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
						RDSize& newOffset ) = 0;
	};
} }

#endif // RD_SDRP_ISERIALIZABLE_H
