#ifndef _IO_h_
#define _IO_h_
	
#include <stm32f10x.h>

#ifdef __cplusplus 
extern "C"
{
#endif

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

#define A15					PORT_B9	
#define A12					PORT_B8	
#define A7					PORT_B7	
#define A6					PORT_B6	
#define A5					PORT_B5	
#define A4					PORT_B4	
#define A3					PORT_B3	
#define A2					PORT_A15
#define A1					PORT_A12
#define A0					PORT_A11
#define IO0					PORT_B14
#define IO1					PORT_B13
#define IO2					PORT_B12

#define WE					PORT_A6
#define A14					PORT_A0
#define A13					PORT_A1
#define A8					PORT_A2
#define A9					PORT_A3
#define A11					PORT_A4
#define OE					PORT_B0
#define A10					PORT_A5 
#define IO7					PORT_B11
#define IO6					PORT_B1
#define IO5					PORT_B10
#define IO4					PORT_A8
#define IO3					PORT_B15


#ifdef __cplusplus 
};
#endif
	
#endif
