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
//	The buffer is empty when the head and tail are equal.
//	The buffer is full when the head is just behind the tail 
//
//  Example (size=8) Normal (tail>head):
//    tail    head    used    free
//      0       0       0       7 empty
//      1       0       1       6
//      2       0       2       5
//      3       0       3       4
//      4       0       4       3
//      5       0       5       2
//      6       0       6       1
//      7       0       7       0 full
//  Example cont'd  Wrapped (head>tail):
//      0       7       7       0 full
//      0       6       6       1
//      0       5       5       2
//      0       4       4       3
//      0       3       3       4
//      0       2       2       5
//      0       1       1       6
//      0       0       0       7 empty
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
    fifo->tail = 0;
	fifo->head = fifo->tail + 1;
    fifo->buf = buf;
}


//-----------------------------------------------------------------------------
//	fifo_Space
//-----------------------------------------------------------------------------
//
//=============================================================================

int16_t  fifo_FreeSpace( FIFO_t * fifo )
{
    return((fifo->head - fifo->tail - 1) & (fifo->size - 1));
}

//-----------------------------------------------------------------------------
//	fifo_PutByte
//-----------------------------------------------------------------------------
//
//=============================================================================

int8_t fifo_PutByte( uint8_t byte, FIFO_t * fifo )
{
	if (0 == fifo_FreeSpace(fifo))
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
        serial_printf("\r\n EMPTY when head is just behind tail.");
        serial_printf("\r\n FULL when head == tail.");
        
        serial_printf("\r\n%02d:EMPTY ", state);
        serial_printf( " h:%d, t:%d,  size:%d, free:%d ", testFifo.head, testFifo.tail, testFifo.size,_FreeSpace(&testFifo));
        state++;
        break;
        
    case 1: //  Normal, add one-byte at a time until buffer is FULL
    case 3: //  Wrapped, add one-byte at a time until buffer is FULL
        result = fifo_PutByte( (uint8_t)0x55, &testFifo);
        if (result == 0)
        {
            serial_printf("\r\n%02d:FULL (%s)", state, (3==state)?"wrapped":"normal");
            state++;
        }
        else 
        {
            serial_printf("\r\n%02d: ", state );
        }
        serial_printf(" %02X  h:%d, t:%d, free:%d ", result, testFifo.head, testFifo.tail, _FreeSpace(&testFifo));
        break;
        
    case 2:     //  Normal, get one-byte at a time until buffer is EMPTY
    case 11:    //  Wrapped, get one-byte at a time until buffer is EMPTY
        result = fifo_GetByte( &byte, &testFifo);
        if (result == -1)
        {
            serial_printf("\r\n%02d:EMPTY", state );
            state++;
        }
        else 
        {
            serial_printf("\r\n%02d:", state );
        }
        serial_printf(" %02X  h:%d, t:%d, free:%d ", result, testFifo.head, testFifo.tail, _FreeSpace(&testFifo));
        break;
        
        
    case 4: //  get one-byte, one
    case 5: //  get one-byte, two
    case 6: //  get one-byte, three
    case 7: //  get one-byte, four
    case 8: //  get one-byte, five
    case 9: //  get one-byte, six
        result = fifo_GetByte( &byte, &testFifo);
        serial_printf("\r\n%02d:%02X  h:%d, t:%d, free:%d ", state, result, testFifo.head, testFifo.tail, _FreeSpace(&testFifo));
        state++;
        break;
   
    case 10: //  Wrapped, add one-byte at a time until buffer is FULL
            //  Should only need to do this for as many bytes as were removed in prior states.
        result = fifo_PutByte( (uint8_t)0xAA, &testFifo);
        if (result == 0)
        {
            serial_printf("\r\n%02d:FULL (wrapped)", state);
            state++;
        }
        else 
        {
            serial_printf("\r\n%02d:", state );
        }
        serial_printf(" %02X  h:%d, t:%d, free:%d ", result, testFifo.head, testFifo.tail, _FreeSpace(&testFifo));
        break;
            
        
    case 12: //  Test Complete
        serial_printf("\r\nTest Complete");
        state++;
        break;
        
    default:
        break;
    }
}

#endif  //  #if 0

// EOF
