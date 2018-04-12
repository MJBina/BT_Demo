//-----------------------------------------------------------------------------
//	Copyright(C) 2014 - Sensata Technologies, Inc.  All rights reserved.
//
//	$Workfile: PIC18_serial.h $
//	$Author: $
//	$Date: $
//	$Revision: $
//
//	Project:
//
//	Description:
// 		PROCESSOR : PIC18F46K22
// 		CLOCK     : INTERNAL
//
//	TODO:
//
//
//=============================================================================

#include "fifo.h"
#include "PIC18_serial.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
// #include <usart.h>
#include <xc.h>


int16_t _serial_getchar(void)
{
    return (EOF);
}

void _serial_HwConfig(void)
{
    TXSTA1 = 0; // Reset USART registers to POR state
    RCSTA1 = 0;

    TXSTA1bits.SYNC = 0;
    TXSTA1bits.BRGH = 0;
    BAUDCON1bits.BRG16 = 0;

    //	if(config&0x08)      // Continuous or single reception
    //		RCSTA1bits.CREN = 1;
    //	else
    //		RCSTA1bits.SREN = 1;

    //	Receive Interrupts?
    PIE1bits.RC1IE = 0;
    //	Transmit Interrupts?
    PIE1bits.TX1IE = 0;

    //-----SPBRG needs to be changed depending upon oscillator frequency-------
    //	SPBRGH1 - EUSART1 Baud Rate Generator, High Byte
    //	SPBRG1 - EUSART1 Baud Rate Generator, Low Byte 
    //	ref Table 16-5 in datasheet:
    //		SYNC = 0, BRGH = 0, BRG16 = 0, Fosc = 64.000 Mhz 
    SPBRGH1 = 0;

    //		9600 baud --> SPBRG1 103 
    //	SPBRG1 = 103;

    //		115.2k baud --> SPBRG1 8
    SPBRG1 = 8;
}

void _serial_Start(void)
{
    PIE1bits.TX1IE = 0;     //  Clear the transmitter interrupt enable
    TXSTA1bits.TXEN = 1;    // Enable transmitter hw
    RCSTA1bits.SPEN = 1;    // Enable receiver hw
}

int16_t _serial_putbuf(unsigned char * data, int len)
{
    do
    { // Transmit a byte
        while ((!TXSTA1bits.TRMT)); //	Check if USART is busy
//        TXREG1 = *data; // 	Write the data byte to the USART
    }
    while (*data++); //	Stop at NUL-Terminator

}

int16_t _serial_getc(void)
{
    return (EOF);
}

//  RETURNS:
//      _serial_putc returns the character written as an unsigned char cast to 
//      an int16 or EOF (-1) on error.
int16_t _serial_putc(char ch)
{
//    TXREG1 = ch;
}

//-----------------------------------------------------------------------------
//  _serial_printf()
//
//  Return Value
//      On success, the total number of characters written is returned.
//      On failure, a negative number is returned.
//-----------------------------------------------------------------------------

#ifndef _OMIT_PRINTF

int16_t _serial_printf (const char * fmt, ...)
{
    char buf[100];  // keep stack usage as small as possible
    va_list args;
	struct	__prbuf	pb;

    int16_t len;

    va_start (args, fmt);
//    len = vsprintf(buf, fmt, args);
//  vsprintf(char * wh, const char * f, va_list ap)
    
	pb.ptr = buf;
	pb.func = (void (*)(char))NULL;
	_doprnt(&pb, fmt, args);
	*pb.ptr = 0;
	len = pb.ptr - buf;
    
    if (len > 0)
    {
        if (0 != _serial_putbuf((unsigned char *)buf, len))
            len = -1;
    }
    va_end (args);
    return(len);
}

#endif  //  _OMIT_PRINTF


void serial_RxIsr(void)
{
    char data;


    if (RCSTA1bits.FERR) // If a framing error occurred
        //    USART1_Status.FRAME_ERROR = 1;    // Set the status bit

        if (RCSTA1bits.OERR) // If an overrun error occurred
            //    USART1_Status.OVERRUN_ERROR = 1;  // Set the status bit

            data = RCREG1; // Read data
}

//-----------------------------------------------------------------------------
//                     T R A N S M I T     I N T E R R U P T
//-----------------------------------------------------------------------------
//
//  From the PIC18(L)F2X/4XK22 User Manual, DS40001412G, page-261. 
//  Section 16.1.1.4 - Transmit Interrupt Flag
//
//  The TXxIF interrupt flag bit of the PIR1/PIR3 register is set whenever the 
//  EUSART transmitter is enabled and no character is being held for trans-
//  mission in the TXREGx. In other words, the TXxIF bit is only clear when the 
//  TSR is busy with a character and a new character has been queued for trans-
//  mission in the TXREGx. 
//
//  The TXxIF flag bit is not cleared immediately upon writing TXREGx. TXxIF 
//  becomes valid in the second instruction cycle following the write execution.
//  Polling TXxIF immediately following the TXREGx write will return invalid 
//  results. The TXxIF bit is read-only, it cannot be set or cleared by software.
//
//  The TXxIF interrupt can be enabled by setting the TXxIE interrupt enable bit 
//  of the PIE1/PIE3 register. However, the TXxIF flag bit will be set whenever 
//  the TXREGx is empty, regardless of the state of TXxIE enable bit.
//
//  To use interrupts when transmitting data, set the TXxIE bit only when there 
//  is more data to send. Clear the TXxIE interrupt enable bit upon writing the 
//  last character of the transmission to the TXREGx.
//
//=============================================================================

void serial_TxIsr(void)
{
    uint8_t tx_byte = 0;
    
    if(-1 == fifo_GetByte( &tx_byte, &TxFifo))
    {
        //  TODO: This is an ERROR
    }
    
    while ((!TXSTA1bits.TRMT)); //	Check if USART is busy
    TXREG1 = tx_byte;           // 	Write the data byte to the USART

    if(!fifo_DataAvailable(&TxFifo))
    {
        //  There is NO MORE data to transmit, clear the interrupt enable bit.
        PIE1bits.TX1IE = 0;
    }
    return;
}


void serial_TxTest(void)
{
    uint8_t tx_byte = 0;
    
    //  Put a message in the buffer
    fifo_PutByte('H', &TxFifo);
    fifo_PutByte('e', &TxFifo);
    fifo_PutByte('l', &TxFifo);
    fifo_PutByte('l', &TxFifo);
    fifo_PutByte('o', &TxFifo);
    fifo_PutByte(' ', &TxFifo);
    fifo_PutByte('W', &TxFifo);
    fifo_PutByte('h', &TxFifo);
    fifo_PutByte('i', &TxFifo);
    fifo_PutByte('r', &TxFifo);
    fifo_PutByte('l', &TxFifo);
    fifo_PutByte('e', &TxFifo);
    fifo_PutByte('d', &TxFifo);
    fifo_PutByte('!', &TxFifo);

    //  Get the first byte
    fifo_GetByte( &tx_byte, &TxFifo);
    //  put it in the transmitter
    TXREG1 = tx_byte;
    //  Enable interrupts - to get next character.
    PIE1bits.TX1IE = 1;
}


// EOF
