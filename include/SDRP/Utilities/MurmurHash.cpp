/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#include <SDRP/Utilities/MurmurHash.h>

namespace Radicle { namespace SDRP
{
	RDUInt32 MurmurHash::Hash( 	const RDUByte8* data, 
					const RDUInt32 dataSize, 
					const RDUInt32 seed )
	{
		RDUInt32 m = 0x5bd1e995;
		RDUInt32 r = 24;

		RDUInt32 h = seed ^ dataSize;

		RDUInt32 len_4 = dataSize >> 2;

		for( RDUInt32 i = 0; i < len_4; i++ ) 
		{
			RDUInt32 i_4 = i << 2;
			RDUInt32 k = data[i_4 + 3];
			k = k << 8;
			k = k | ( data[i_4 + 2] & 0xff );
			k = k << 8;
			k = k | ( data[i_4 + 1] & 0xff );
			k = k << 8;
			k = k | ( data[i_4 + 0] & 0xff );
			k *= m;
			k ^= k >> r;
			k *= m;
			h *= m;
			h ^= k;
		}

		RDUInt32 len_m = len_4 << 2;
		RDUInt32 left = dataSize - len_m;

		if( left != 0 ) 
		{
			if( left >= 3 ) 
			{
				h ^= (RDUInt32) data[dataSize - 3] << 16;
			}
			if( left >= 2 ) 
			{
				h ^= (RDUInt32) data[dataSize - 2] << 8;
			}
			if( left >= 1 ) 
			{
				h ^= (RDUInt32) data[dataSize - 1];
			}

			h *= m;
		}

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	}
} }
 
