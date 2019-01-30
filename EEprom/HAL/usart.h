#ifndef _USART_h_
#define _USART_h_

#ifdef __cplusplus 
extern "C"
{
#endif
	
#include <stm32f10x.h>

extern void USART1_Init(int baud);
extern void USART1_printf(char *fmt, ...);
extern void USART1_IRQHandler(void);
	
#ifdef __cplusplus 
};
#endif

#endif // _USART_h_
