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

#include "PIC18_serial.h"

#include <stdint.h>
// #include <usart.h>
#include <xc.h>


#ifndef EOF
#define	EOF	((int16_t)(-1))
#endif	//	EOF

int16_t _serial_getchar(void)
{
    return (EOF);
}

void _serial_Config(void)
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
    TXSTA1bits.TXEN = 1; // Enable transmitter
    RCSTA1bits.SPEN = 1; // Enable receiver
}

int16_t _serial_TxBuf(unsigned char *data, int len)
{
    do
    { // Transmit a byte
        while ((!TXSTA1bits.TRMT)); //	Check if USART is busy
        TXREG1 = *data; // 	Write the data byte to the USART
    }
    while (*data++); //	Stop at NUL-Terminator

}

int16_t _serial_getc(void)
{
    return (EOF);
}

int16_t _serial_putc(char ch)
{
    TXREG1 = ch;
}

int16_t _serial_printf(char * fmt, ...)
{
    return (EOF);
}

void serial_RxIsr(void)
{
    char data;


    if (RCSTA1bits.FERR) // If a framing error occured
        //    USART1_Status.FRAME_ERROR = 1;    // Set the status bit

        if (RCSTA1bits.OERR) // If an overrun error occured
            //    USART1_Status.OVERRUN_ERROR = 1;  // Set the status bit

            data = RCREG1; // Read data
}

void serial_TxIsr(void)
{
}

// EOF
