/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Fubarino SD (1.5), Platform=pic32, Package=chipKIT
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define __PIC32_32MX795F512H__
#define __PIC32MX__
#define __32MX795F512H__
#define __LANGUAGE_C_PLUS_PLUS__
#define ARDUINO_ARCH_PIC32
#define F_CPU 80000000L
#define ARDUINO 10607
#define _BOARD_FUBARINO_SD_
#define MPIDEVER 16777998
#define MPIDE 150
#define IDE Arduino
#define _USE_USB_FOR_SERIAL_
#define __cplusplus 201103L
#define GCC_VERSION 40803

#define __inline__
#define __asm__(x)
#define __asm__(char)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __volatile__


#define __attribute__(x)
#define __LANGUAGE_C_PLUS_PLUS

typedef void *__builtin_va_list;

typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() { ; }
#include <arduino.h>
#include <Board_Defs.h> 
#undef F
#define F(string_literal) ((const PROGMEM char *)(string_literal))
#undef PSTR
#define PSTR(string_literal) ((const PROGMEM char *)(string_literal))
#undef cli
#define cli()
#define pgm_read_byte(address_short)
#define pgm_read_word(address_short)
#define pgm_read_word2(address_short)
#define digitalPinToPort(P)
#define digitalPinToBitMask(P) 
#define digitalPinToTimer(P)
#define analogInPinToBit(P)
#define portOutputRegister(P)
#define portInputRegister(P)
#define portModeRegister(P)
#include <..\IREC_2017\IREC_2017.ino>
#include <..\IREC_2017\BMP180.cpp>
#include <..\IREC_2017\BMP180.h>
#include <..\IREC_2017\DataLogger.cpp>
#include <..\IREC_2017\DataLogger.h>
#include <..\IREC_2017\EM_506.cpp>
#include <..\IREC_2017\EM_506.h>
#include <..\IREC_2017\L3GD20TR.cpp>
#include <..\IREC_2017\L3GD20TR.h>
#include <..\IREC_2017\LIS331HH.cpp>
#include <..\IREC_2017\LIS331HH.h>
#include <..\IREC_2017\Parameters.cpp>
#include <..\IREC_2017\Parameters.h>
#include <..\IREC_2017\WSU_mavlink.cpp>
#include <..\IREC_2017\WSU_mavlink.h>
#endif
