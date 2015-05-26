/************************************************************************
 * Radicle Design		   					*
 * Service Discovery Routing Protocol					*
 *									*
 * Author: 	Warren Kenny	<warren.kenny@gmail.com>		*
 * 									*
 * Copyright 2010 Radicle Design. All rights reserved.			*
 ************************************************************************/
 
#ifndef RD_SDRP_ERROR_CODES_H
#define RD_SDRP_ERROR_CODES_H

#define RD_SDRP_ERROR_PACKET_TYPE		0
#define RD_SDRP_ERROR_SERIALIZATION_FAILURE	RD_SDRP_ERROR_PACKET_TYPE + 1
#define RD_SDRP_ERROR_DESERIALIZATION_FAILURE	RD_SDRP_ERROR_SERIALIZATION_FAILURE + 1
#define RD_SDRP_ERROR_FILTER_MISMATCH		RD_SDRP_ERROR_DESERIALIZATION_FAILURE + 1
#define RD_SDRP_ERROR_FILTER_SIZE		RD_SDRP_ERROR_FILTER_MISMATCH + 1

#endif // RD_SDRP_ERROR_CODES_H
	
