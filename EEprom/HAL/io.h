#ifndef _IO_h_
#define _IO_h_
	
#include "stm32f10x.h"

#ifdef __cplusplus 
extern "C"
{
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef struct{
    GPIO_TypeDef* type;
    uint16_t pin;
    uint32_t rcc;
}GPIO;

void pinMode(const GPIO* gpio, int mode);
#define IO_MODE(n, v) pinMode((&(n)), (v))
#define IO_WRITE(n, v) (((v)==0) ? GPIO_ResetBits(n.type, n.pin) : GPIO_SetBits(n.type, n.pin))
#define IO_READ(n) GPIO_ReadInputDataBit(n.type, n.pin)
//#define IO_DELAY_MS(ms) delay_ms((ms))
//#define IO_DELAY_US(us) delay_us((us))

// emulate 8051
extern const GPIO PORT_A0;
extern const GPIO PORT_A1;
extern const GPIO PORT_A2;
extern const GPIO PORT_A3;
extern const GPIO PORT_A4;
extern const GPIO PORT_A5;
extern const GPIO PORT_A6;
extern const GPIO PORT_A7;

extern const GPIO PORT_A8 ;
extern const GPIO PORT_A9 ;
extern const GPIO PORT_A10;
extern const GPIO PORT_A11;
extern const GPIO PORT_A12;
extern const GPIO PORT_A13;
extern const GPIO PORT_A14;
extern const GPIO PORT_A15;

extern const GPIO PORT_B0 ;
extern const GPIO PORT_B1 ;
extern const GPIO PORT_B2 ;
extern const GPIO PORT_B3 ;
extern const GPIO PORT_B4 ;
extern const GPIO PORT_B5 ;
extern const GPIO PORT_B6 ;
extern const GPIO PORT_B7 ;

extern const GPIO PORT_B8 ;
extern const GPIO PORT_B9 ;
extern const GPIO PORT_B10;
extern const GPIO PORT_B11;
extern const GPIO PORT_B12;
extern const GPIO PORT_B13;
extern const GPIO PORT_B14;
extern const GPIO PORT_B15;

#define A15					PORT_A0		// P1.0
#define A12					PORT_A1		// P1.1
#define A7					PORT_A2		// P1.2
#define A6					PORT_A3		// P1.3
#define A5					PORT_A4		// P1.4
#define A4					PORT_A5		// P1.5
#define A3					PORT_A6		// P1.6
#define A2					PORT_A7		// P1.7
#define A1					PORT_B0		// P3.2
#define A0					PORT_B1		// P3.3
#define IO0					PORT_B10	// P3.5
#define IO1					PORT_B11	// P3.6
#define IO2					PORT_B11	// P3.6

#define WE					PORT_B9  //  P0.4
#define A14					PORT_B8  //  P0.2
#define A13					PORT_B7  /// P2.0
#define A8					PORT_B6  /// P2.1
#define A9					PORT_B5  /// P0.6
#define A11					PORT_B4  //  P0.3 // 
#define OE					PORT_B4  //  P0.3 // 
#define A10					PORT_B9  //  P0.4
#define IO7					PORT_B4  //  P0.3 // 
#define IO6					PORT_B5  /// P0.6
#define IO5					PORT_B6  /// P2.1
#define IO4					PORT_B7  /// P2.0
#define IO3					PORT_B8  //  P0.2

#ifdef __cplusplus 
};
#endif
	
#endif
