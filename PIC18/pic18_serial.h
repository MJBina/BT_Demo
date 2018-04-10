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

#ifndef __PIC18_SERIAL_H
#define __PIC18_SERIAL_H

#include <stdint.h>
#include <stdio.h>		//	for EOF

#ifndef EOF
  #define	EOF	((int16_t)(-1))
#endif	//	EOF

//  Interface functions - needed by PIC18 ISR Handler

extern void serial_RxIsr(void);

extern void serial_TxIsr(void);

//  Initialization Functions 

extern void _serial_HwConfig(void);

extern void _serial_Start(void);


// Application Interface functions
extern int16_t _serial_getchar(void);

extern int16_t _serial_TxBuf(unsigned char *data, int len);

extern int16_t _serial_getc(void);

extern int16_t _serial_putc(char ch);

extern int16_t _serial_printf(char * fmt, ...);



//	The following macros are implemented so that serial functions will only be 
//	called if '__DEBUG' is defined. __DEBUG is defined if building the DEBUG 
//	version within the IDE.
//
//	Note that serial_printf is a VARIADIC macro, accepting optional multiple 
//	arguments. For a good explanation of variadic macros, refer to the link:
//		http://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html#Variadic-Macros

// #ifdef __DEBUG
#if 1
#define serial_Config()	_serial_Config()
#define serial_Start() _serial_Start()
#define serial_TxBuf(data, len) _serial_TxBuf(data, len)
//  #define serial_printf(format, ...) 	_serial_printf(format, ##__VA_ARGS__)
#define serial_printf 	_serial_printf
#define serial_getc()	_serial_getc()
#define serial_putc(ch)	_serial_putc(ch)
#define serial_HwConfig()   _serial_HwConfig()
#define serial_Start()   _serial_Start()

#else
#define serial_Config()	
#define serial_Start() 
#define serial_TxBuf(data, len) 	//	(unsigned char *data, int len)
#define serial_printf(format, ...)		//	(format, ##__VA_ARGS__)
#define serial_getc()
#define serial_putc(ch)
#endif


#endif //	__PIC18_SERIAL_H

// EOF
//*****************************************************************************
//	$Log: $
// 
