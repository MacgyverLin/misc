#ifndef __DELAY_H
#define __DELAY_H 			   

#include <stm32f10x.h>

#ifdef __cplusplus 
extern "C"
{
#endif

#define DATA_SETUP_TIME 5

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#ifdef __cplusplus 
};
#endif
	
#endif

