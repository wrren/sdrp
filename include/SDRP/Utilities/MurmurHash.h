/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_MURMUR_HASH_H
#define RD_SDRP_MURMUR_HASH_H

#include <SDRP/Core/Types.h>

namespace Radicle { namespace SDRP
{
	/**
	 *	Simple Non-Cryptographic Hash Implementation for General Lookup
	 */
	class MurmurHash
	{
	public:
	
		/**
		 *	Generate a 32-Bit Hash from the provided data, using the seed for randomization
		 * @param data		Data Buffer 
		 * @param dataSize	Data Buffer Size in Bytes
		 * @param seed		Hash Seed
		 * @return		32-Bit Hash
		 */
		static RDUInt32 Hash( const RDUByte8* data, const RDUInt32 dataSize, const RDUInt32 seed );
		
		/**
		 *	Generate a 32-Bit Hash from the provided data, using the seed for randomization
		 * @param data		Data 
		 * @param seed		Hash Seed
		 * @return		32-Bit Hash
		 */
		template<typename T>
		static RDUInt32 Hash( const T& data, const RDUInt32 seed )
		{
			return Hash( reinterpret_cast<const RDUByte8*>( &data ), sizeof( T ), seed );
		}
	};
} }

#endif // RD_SDRP_MURMUR_HASH_H
 
