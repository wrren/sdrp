/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * Platforms:	ns-2, Unix, Windows					*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/

#ifndef RD_SDRP_TYPES_H
#define RD_SDRP_TYPES_H

// Single-Byte Types
typedef unsigned char		RDUByte8;
typedef char			RDByte8;
typedef unsigned char		RDUChar8;
typedef char			RDChar8;
typedef unsigned char		RDUInt8;
typedef char			RDInt8;

// Double-Byte Types
typedef unsigned short		RDUInt16;
typedef short			RDInt16;

// Quad-Byte Types
typedef unsigned int		RDUInt32;
typedef int			RDInt32;
typedef unsigned int		RDUInt;
typedef int			RDInt;

// Size Type
typedef unsigned long long	RDSize;

// Floating Point Types
typedef double			RDDouble;
typedef float			RDFloat;

// Time Types
typedef double			RDTimeStamp;

// Primitive Address Types
typedef RDUInt16		RDNetworkAddress;

// Service Identifier
typedef RDUInt16		RDServiceIdentifier;

/// Generic Identifier
typedef RDUInt16		RDIdentifier;

// Composite Type
typedef RDSize			RDComposite;

// Timestamp
typedef double			RDTimeStamp;
typedef double 			RDTime;

#endif // RD_SDRP_TYPES_H

