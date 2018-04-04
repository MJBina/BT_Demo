/* 
 * File:   fifo.c
 * Author: A1021170
 *
 * Created on April 4, 2018, 3:59 PM
 */

//-----------------------------------------------------------------------------
//	Copyright(C) 2012 - Sensata Technologies, Inc.  All rights reserved.
//
//	$Workfile: fifo.c $	 
//	$Author: a1021170 $	
//	$Date: 2012-02-17 15:29:19-06:00 $
//	$Revision: 2 $ 
//
//	Project:
//		RS232 Communications
//
//	Description:
//		This code implements a FIFO buffer as a circular queue.  
//		Data is serviced from the Head of the queue
//		Data is added at the Tail of the queue
//		The Tail is always left pointing to the next location to be filled.
//		The buffer is empty when the Head and Tail point to the same location.
//		The buffer is full when the Head is just behind the Tail 
//	
//
//=============================================================================

#include "fifo.h"

#include <stdint.h>

#define BUF_SIZE	0x0FF
#define BUF_MASK	0x0FF


unsigned char Buf[BUF_SIZE];
int16_t	Head = 0;
int16_t	Tail = 0;
int16_t FreeSpace = 0;	

//-----------------------------------------------------------------------------
//	fifo_Init
//-----------------------------------------------------------------------------
//
//=============================================================================
void fifo_Init( void )
{
	//	The buffer is empty when the Head and Tail point to the same location.
	Head = Tail = 0;
	FreeSpace = BUF_SIZE;
}

//-----------------------------------------------------------------------------
//	fifo_Space
//-----------------------------------------------------------------------------
//
//=============================================================================
static int16_t fifo_FreeSpace( void )
{
	return( FreeSpace );
}


//-----------------------------------------------------------------------------
//	fifo_PutByte
//-----------------------------------------------------------------------------
//
//=============================================================================
int8_t fifo_PutByte( uint8_t byte )
{
	if (0 == FreeSpace)
	{	
		return ( 0 );
	}
	else
	{
		Buf[Tail++] = byte;
		Tail &= BUF_MASK; 
		FreeSpace--;
		return( 1 );
	}
}

//-----------------------------------------------------------------------------
//	fifo_GetByte
//-----------------------------------------------------------------------------
//
//=============================================================================
int16_t fifo_GetByte( uint8_t * byte )
{
	if ( BUF_SIZE == FreeSpace )
	{
		return( -1 );
	}
	else
	{
		*byte = Buf[Head++];
		Head &= BUF_MASK;
		FreeSpace++;
		return( byte );
	}
}

// EOF
