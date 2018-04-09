//	Copyright Mark J. Bina, 2018 
//
// File:   fifo.c
// Author: Mark J. Bina
//-----------------------------------------------------------------------------
//
//	TODO: Understand how Git implements Keywords (below)
//		$Workfile: $	 
//		$Author: $	
//		$Date: $
//		$Revision: $ 
//
//	DESCRIPTION:
//	This code implements a FIFO buffer as a circular queue.  
//	Items are added to the queue at the 'tail' and removed from the 'head'.  
//	This is analogous to waiting in line. You start at the end (tail) of the 
//	line and receive some service at the head of the line.
//	
//	The head is the index of the next location to be written.
//	The tail is the index of the next loaction to be read from.	
//	The queue is empty (no data to be read) when the head and tail are equal.
//	The buffer is full when the head is just behind the tail 
//
//	Powers of 2:
//	When the buffer size is a power-of-2, calculations are much simpler and more
//	reliable.
//
//=============================================================================

#define FIFO_ALLOCATE_BUFFER_SPACE
#include "fifo.h"
#undef FIFO_ALLOCATE_BUFFER_SPACE

#include <stdint.h>

//-----------------------------------------------------------------------------
//	fifo_Init
//-----------------------------------------------------------------------------
//
//=============================================================================
void fifo_Init( FIFO_t * fifo, uint8_t * buf )
{
	//	The buffer is empty when the Head and Tail point to the same location.
	fifo->head = fifo->tail = 0;
    fifo->buf = buf;
}

//-----------------------------------------------------------------------------
//	fifo_Space
//-----------------------------------------------------------------------------
//
//=============================================================================

int16_t _FreeSpace( FIFO_t * fifo )
{
//    if (IS_RXBUF_EMPTY()) return(RXBUF_SIZE-1);   // one less than full buffer
    if ( fifo->tail > fifo->head )
	{
        return(fifo->size - fifo->tail + fifo->head - 1);
	}
	else
	{
		return( fifo->head - fifo->tail - 1);
	}
}

static int16_t fifo_FreeSpace( FIFO_t * fifo )
{
	return( _FreeSpace(fifo) );
}






//-----------------------------------------------------------------------------
//	fifo_PutByte
//-----------------------------------------------------------------------------
//
//=============================================================================
int8_t fifo_PutByte( uint8_t byte, FIFO_t * fifo )
{
	if (0 == _FreeSpace(fifo))
	{	
		return ( 0 );
	}
	else
	{
		fifo->buf[fifo->tail++] = byte;
		fifo->tail &= (fifo->size - 1); 
		return( 1 );
	}
}

//-----------------------------------------------------------------------------
//	fifo_GetByte
//-----------------------------------------------------------------------------
//
//=============================================================================
#define FIFO_DATA_AVAIL(fifo)   1

int16_t fifo_GetByte( uint8_t * byte, FIFO_t * fifo )
{
	if ( !FIFO_DATA_AVAIL(fifo) )
	{
		return( -1 );
	}
	else
	{
		*byte = fifo->buf[fifo->head++];
		fifo->head &= (fifo->size - 1);
		return((int16_t)(*byte));
	}
}

// EOF
