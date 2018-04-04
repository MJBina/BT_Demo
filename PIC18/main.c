//-----------------------------------------------------------------------------
//	Copyright(C) 2014 - Your Development Team LLC.  All rights reserved.
//
//	$Workfile: $
//	$Author: $
//	$Date: $
//	$Revision: $ 
//
//	Project:	
//
//	Description:
//		COMPILER  	: MPLAB® C18 C Compiler, v3.10, 16 February 2007
//					: Part Number: SW006011
// 		PROCESSOR 	: PIC18F46K22 44-pin TQFP
// 		CLOCK     	: Internal 
//
//
//	TODO:
//
//=============================================================================

//-- PIC18F46K22 CONFIGURATION ------------------------------------------------
//		NOTE: I copied this from some demo code...to get things started. I have
//			 not verified its correctness, or applicability  -mjb-
//		Ref: PIC18F46K22.INC for details

#pragma config FOSC = INTIO67   // Internal OSC block, Port Function on RA6/7
#pragma config PLLCFG = ON     	// Oscillator multiplied by 4
#pragma config PRICLKEN = OFF   // Primary clock can be disabled by software
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF       // OSC switch mode disabled
#pragma config PWRTEN = OFF     // Power Up Timer is disabled
#pragma config BOREN = OFF      // Brown-out Reset disabled in Hardware and Soft
#pragma config BORV = 250       // VBOR set to 2.50 V nominal
#pragma config WDTEN = OFF      // Watch Dog Timer disabled. SWDTEN no effect
#pragma config WDTPS = 1        // Watch Dog Timer Postscale 1:1
#pragma config PBADEN = OFF     // PORTB<5:0> pins are Config as Digital I/O on Reset
#pragma config CCP2MX = PORTB3  // CCP2 input/output is multiplexed with RB3
#pragma config CCP3MX = PORTB5	//* CCP3 input/output is multiplexed with RB5
#pragma config HFOFST = OFF     // HFINTOSC ouput and ready status are delayed by OSC stable status
#pragma config T3CMX = PORTB5   // T3CKl is on RB5
#pragma config P2BMX = PORTC0   // P2B is on RC0
#pragma config MCLRE = EXTMCLR  // MCLR pin enabled, RE3 input pin disabled
#pragma config STVREN = OFF     // Stack full/underflow will not cause Reset
#pragma config LVP = OFF        // Single-Supply ICSP disabled
#pragma config XINST = OFF      // Instruction set Extension and indexed Addressing mode disabled
#pragma config DEBUG = OFF      // Disabled
#pragma config CP0 = OFF        // Block 0 (000800 - 001FFFh) not code-protected
#pragma config CP1 = OFF        // Block 1 (002000 - 003FFFh) not code-protected
#pragma config CP2 = OFF        // Block 2 (004000 - 005FFFh) not code-protected
#pragma config CP3 = OFF        // Block 3 (006000 - 007FFFh) not code-protected
#pragma config CPB = OFF        // Boot Block (000000 - 0007FFh) not code-protected
#pragma config CPD = OFF        // Data EEPROM not code-protected
#pragma config WRT0 = OFF       // Block 0 (000800-001FFFh) not write-protected
#pragma config WRT1 = OFF       // Block 1 (002000-003FFFh) not write-protected
#pragma config WRT2 = OFF       // Block 2 (004000-005FFFh) not write-protected
#pragma config WRT3 = OFF       // Block 3 (006000-007FFFh) not write-protected
#pragma config WRTC = OFF       // Configuration registers (300000-3000FFh) not write protected
#pragma config WRTB = OFF       // Boot Block (000000-0007FFh) not write-protected
#pragma config WRTD = OFF       // Data EEPROM not write-protected
#pragma config EBTR0 = OFF      // Block 0 (000800 - 001FFFh) not protected from table reads executed in other blocks
#pragma config EBTR1 = OFF      // Block 0 (002000 - 003FFFh) not protected from table reads executed in other blocks
#pragma config EBTR2 = OFF      // Block 0 (004000 - 005FFFh) not protected from table reads executed in other blocks
#pragma config EBTR3 = OFF      // Block 0 (006000 - 007FFFh) not protected from table reads executed in other blocks
#pragma config EBTRB = OFF      // Block 0 (000000 - 0007FFh) not protected from table reads executed in other blocks

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


#define ALLOCATE_SPACE
#undef ALLOCATE_SPACE

#include "PIC18_serial.h"

#include <stdint.h>
#include <stdio.h>



//  RC6 (pin-44) LED6 (0=inactive)
#define IS_LED06_ON()        (1==LATDbits.LATD6)
#define LED06_SET(onoff)     (LATDbits.LATD6 = ((onoff)?1:0))
#define LED06_ON()           LED06_SET(1)
#define LED06_OFF()          LED06_SET(0)
#define LED06_TOGGLE()       LED06_SET(!IS_LED06_ON())

//  RC7 (pin-1) LED07 (0=inactive)
#define IS_LED07_ON()        (1==LATDbits.LATD7)
#define LED07_SET(onoff)     (LATDbits.LATD7 = ((onoff)?1:0))
#define LED07_ON()           LED07_SET(1)
#define LED07_OFF()          LED07_SET(0)
#define LED07_TOGGLE()       LED07_SET(!IS_LED07_ON())



//-----------------------------------------------------------------------------
//	_HwConfig()
//-----------------------------------------------------------------------------
//	Hardware Configuration for the roboMech board
//
//		CCP1/RC2, pin-36 - PWM1 output
//		CCP2/RB3, pin-11 - PWM2 output
//		CCP3/RB5, pin-15 - PWM3 output
//		CCP4/RD1, pin-39 - PWM4 output
//
//=============================================================================

static void _HwConfig(void)
{
    // 	OSCILLATOR CONFIGURATION
    //	64MHz - Highest speed, Internal Oscillator, Full-power

    //	Main System Clock Selection (SCS)
    //	SCS<1:0>: System Clock Select bit
    //		1x = Internal oscillator block
    //		01 = Secondary (SOSC) oscillator
    //		00 = Primary clock (determined by FOSC<3:0> in CONFIG1H).
    OSCCONbits.SCS = 0b00;

    //	Internal Frequency selection bits (IRCF, INTSRC)
    //	IRCF<2:0>: Internal RC Oscillator Frequency Select bits(2)
    //		111 = HFINTOSC – (16 MHz)
    //		110 = HFINTOSC/2 – (8 MHz)
    //		101 = HFINTOSC/4 – (4 MHz)
    //		100 = HFINTOSC/8 – (2 MHz)
    //		011 = HFINTOSC/16 – (1 MHz)(3)
    OSCCONbits.IRCF = 0b111;

    //	Primary Clock Frequency 4x multiplier (PLLEN)
    //	PLLEN: Frequency Multiplier 4xPLL for HFINTOSC Enable bit(1)
    //		1 = PLL enabled
    //		0 = PLL disabled
    OSCTUNEbits.PLLEN = 1;

    //	PORT-B 

    //	Port-C
    TRISCbits.TRISC6 = 0; //	TX1/RC6		pin-44		Serial Data Transmit
    TRISCbits.TRISC7 = 1; //	RX1/RC7		pin-1		Serial Data Receive

    // Port-D
    TRISDbits.TRISD6 = 0; //	RD6/P1C		pin-4		LED
    TRISDbits.TRISD7 = 0; //	RD7/P1D		pin-5		LED
}


//-----------------------------------------------------------------------------
//	_T2Config
//-----------------------------------------------------------------------------
//	Initialize Timer2 to PWM period
//	Goal: PWM Period = xx mSec (yy Hz), Resolution = zz
//
//	Timer2 input clock is Fosc/4.  Fosc = 64MHz --> input clock = 16MHz
//
//	The timer module is an 8-bit timer/counter affected by the following 
//	registers:
//		• TMRx: 8-bit timer count register
//		• PRx: 8-bit period register associated with the timer
//		• TxCON: 8-bit control register associated with the timer
//
//	Timer2 clock input is the system instruction clock (FOSC/4).
//		
//=============================================================================

void _T2Config(void)
{
    // TxCKPS<1:0>: Timer2-type Clock Prescale Select bits
    //		00 = Prescaler is 1
    //		01 = Prescaler is 4
    //		1x = Prescaler is 16
    T2CONbits.T2CKPS = 0b10;

    //	Clear the timer register
    TMR2 = 0;

    //	PR2: 8-bit period register for Timer-2
    PR2 = 200;
}

//-----------------------------------------------------------------------------
//	_T2Start
//-----------------------------------------------------------------------------
//	Start the timer 
//	
//=============================================================================

void _T2Start(void)
{
    PIR1bits.TMR2IF = 0; // Clear Timer2 Interrupt Flag
    PIE1bits.TMR2IE = 1; // Enable Interrupts for Timer2

    T2CONbits.TMR2ON = 1; // turn on timer 2 
}

int T2TickCount = 0;




//.	//----------------------------------------------------------------------------
//.	// High priority interrupt vector
//.	
//.	void InterruptHandlerHigh(void);
//.	
//.	#pragma code InterruptVectorHigh = 0x08
//.	void InterruptVectorHigh (void)
//.	{
//.	  _asm
//.	    goto InterruptHandlerHigh //jump to interrupt routine
//.	  _endasm
//.	}
//.	
//.	//----------------------------------------------------------------------------
//.	// High priority interrupt routine
//.	
//.	#pragma code
//.	#pragma interrupt InterruptHandlerHigh
//.	
//.	void InterruptHandlerHigh(void) 
//.	{
//.		if(PIR1bits.TMR2IF) {
//.			PIR1bits.TMR2IF = 0;
//.			T2TickCount++;
//.		}
//.		if(PIR1bits.RC1IF) {
//.			//	RC1IF: EUSART1 Receive Interrupt Flag bit
//.			//	1 = The EUSART1 receive buffer, RCREG1, is full (cleared when RCREG1 is read)
//.			//	0 = The EUSART1 receive buffer is empty
//.			serial_RxIsr();
//.		}
//.		if(PIR1bits.TX1IF) {
//.			//	TX1IF: EUSART1 Transmit Interrupt Flag bit
//.			//	1 = The EUSART1 transmit buffer, TXREG1, is empty (cleared when TXREG1 is written)
//.			//	0 = The EUSART1 transmit buffer is full
//.			serial_TxIsr();
//.		}
//.		if(PIR1bits.SSP1IF) {
//.			//	SSP1IF: Master Synchronous Serial Port 1 Interrupt Flag bit
//.			//	1 = The transmission/reception is complete (must be cleared by software)
//.			//	0 = Waiting to transmit/receive
//.			PIR1bits.SSP1IF = 0;
//.		}
//.		
//.	}

//----------------------------------------------------------------------------

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

void interrupt high_isr(void)
{
    if (PIR1bits.TMR2IF)
    {
        PIR1bits.TMR2IF = 0;
        T2TickCount++;
    }

    if (PIR1bits.RC1IF)
    {
        //	RC1IF: EUSART1 Receive Interrupt Flag bit
        //	1 = The EUSART1 receive buffer, RCREG1, is full (cleared when RCREG1 is read)
        //	0 = The EUSART1 receive buffer is empty
        serial_RxIsr();
    }

    if (PIR1bits.TX1IF)
    {
        //	TX1IF: EUSART1 Transmit Interrupt Flag bit
        //	1 = The EUSART1 transmit buffer, TXREG1, is empty (cleared when TXREG1 is written)
        //	0 = The EUSART1 transmit buffer is full
        serial_TxIsr();
    }

    if (PIR1bits.SSP1IF)
    {
        //	SSP1IF: Master Synchronous Serial Port 1 Interrupt Flag bit
        //	1 = The transmission/reception is complete (must be cleared by software)
        //	0 = Waiting to transmit/receive
        PIR1bits.SSP1IF = 0;
    }

    /* This code stub shows general interrupt handling.  Note that these
    conditional statements are not handled within 3 seperate if blocks.
    Do not use a seperate if block for each interrupt flag to avoid run
    time errors. */

#if 0

    /* TODO Add High Priority interrupt routine code here. */

    /* Determine which flag generated the interrupt */
    if (<Interrupt Flag 1 >)
    {
        <Interrupt Flag 1 = 0 >; /* Clear Interrupt Flag 1 */
    }
    else if (<Interrupt Flag 2 >)
    {
        <Interrupt Flag 2 = 0 >; /* Clear Interrupt Flag 2 */
    }
    else
    {
        /* Unhandled interrupts */
    }

#endif

}

/* Low-priority interrupt routine */
void low_priority interrupt low_isr(void)
{
    /* This code stub shows general interrupt handling.  Note that these
    conditional statements are not handled within 3 seperate if blocks.
    Do not use a seperate if block for each interrupt flag to avoid run
    time errors. */

#if 0

    /* TODO Add Low Priority interrupt routine code here. */

    /* Determine which flag generated the interrupt */
    if (<Interrupt Flag 1 >)
    {
        <Interrupt Flag 1 = 0 >; /* Clear Interrupt Flag 1 */
    }
    else if (<Interrupt Flag 2 >)
    {
        <Interrupt Flag 2 = 0 >; /* Clear Interrupt Flag 2 */
    }
    else
    {
        /* Unhandled interrupts */
    }

#endif

}

void main(void)
{
    int tick = 0;
    int led_tick = 0;


    INTCONbits.GIE = 0; //	disable interrupts
    RCONbits.IPEN = 0; //	disable interrupt priority levels

    _HwConfig();

    LED06_ON();
    LED07_ON();

    _T2Config();
    serial_Config();

    //	enable interrupts
    INTCONbits.PEIE = 1; //	Peripheral Interrupt Enable
    INTCONbits.GIE = 1; //	Global Interrupt Enable

    _T2Start();
    serial_Start();

    while (1)
    {
        //	Timer2 is running at 5KHz.  T2TickCount increments at 200 uSec
        if (T2TickCount > 0)
        {
            T2TickCount--;

            if (++tick >= 2)
            {
                // this loop runs approximately once per millisecond
                tick = 0;
                //				motor_test();
                led_tick++;
                if (250 == led_tick)
                {
                    LED06_OFF();
                    LED07_OFF();
                    serial_putc('-');
                }
                else if (500 == led_tick)
                {
                    LED06_ON();
                    LED07_OFF();
                }
                else if (750 == led_tick)
                {
                    LED06_ON();
                    LED07_ON();
                    serial_putc('+');
                }
                else if (led_tick >= 1000)
                {
                    LED06_OFF();
                    LED07_ON();
                    led_tick = 0;
                }
            }
        }
    }
}
