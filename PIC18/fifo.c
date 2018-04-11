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
//	The tail is the index of the next location to be read from.	
//	The buffer is full when the head and tail are equal.
//	The queue is empty (no data to be read) when the head is just behind the tail 
//
//  Example (size=8) Normal (tail>head):
//    tail    head    used    free
//      1       0       0       7 empty
//      2       0       1       6
//      3       0       2       5
//      4       0       3       4
//      5       0       4       3
//      6       0       5       2
//      7       0       6       1
//      0       0       7       0 full
//  Example contd' Wrapped (head>tail):
//      0       7       0       7 empty
//      0       6       1       6
//      0       5       2       5
//      0       4       3       4
//      0       3       4       3
//      0       2       5       2
//      0       1       6       1
//      0       0       7       0 full
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
//  The FIFO is initialized at startup.  Therefore, it is not necessary to calloc
//  this function. It is provided here as a means to re-initialize.
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

static int16_t _FreeSpace( FIFO_t * fifo )
{

    if ( fifo->tail >= fifo->head )     //  Normal
	{
        return((fifo->size - 1) - (fifo->tail - fifo->head));
	}
	else    //  Wrapped
	{
		return( fifo->head - fifo->tail - 1);
	}
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

#define FIFO_DATA_AVAIL(fifo)   ((_FreeSpace(fifo) == (fifo->size -1)) ? 0:1)

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


#if 0

#include "pic18_serial.h"

//	Test FIFO buffer.
#define TEST_BUF_SIZE   16
uint8_t testBuf[TEST_BUF_SIZE];
FIFO_t testFifo = { testBuf, TEST_BUF_SIZE, 0, 0 }; 

extern int16_t _serial_printf (const char * fmt, ...);

void fifo_FreeSpaceTest(void)
{
    static int16_t state = 0;
    static int16_t result = 0;
    uint8_t byte = 0;
    
    switch (state)
    {
    case 0: //  initial, empty
        serial_printf("\r\n%02d:xx  h:%d, t:%d, free:%d ", state, testFifo.head, testFifo.tail, _FreeSpace(&testFifo));
        state++;
        break;
        
    case 1: //  Normal, add one-byte at a time until buffer is full
        result = fifo_PutByte( (uint8_t)0x55, &testFifo);
        serial_printf("\r\n%02d:%02X  h:%d, t:%d, free:%d ", state, result, testFifo.head, testFifo.tail, _FreeSpace(&testFifo));
        if (result == 0)
            state++;
        break;
        
    case 2: //  get one-byte, one
    case 3: //  get one-byte, two
    case 4: //  get one-byte, three
    case 5: //  get one-byte, four
        result = fifo_GetByte( &byte, &testFifo);
        serial_printf("\r\n%02d:%02X  h:%d, t:%d, free:%d ", state, result, testFifo.head, testFifo.tail, _FreeSpace(&testFifo));
        if (result == -1)
            state++;
        break;
   
    case 6: //  Wrapped, add one-byte at a time until buffer is full
        result = fifo_PutByte( (uint8_t)0xAA, &testFifo);
        serial_printf("\r\n%02d:%02X  h:%d, t:%d, free:%d ", state, result, testFifo.head, testFifo.tail, _FreeSpace(&testFifo));
        if (result == 0)
            state++;
        break;
        
    case 7: //  Test Complete
        serial_printf("\r\nTest Complete");
        state++;
        break;
        
    default:
        break;
    }
}

#endif  //  #if 0

// EOF
