#ifndef _TIMER_h_
#define _TIMER_h_

#include "VGMDEF.h"

#ifdef __cplusplus 
extern "C"
{
#endif

extern void TIMER_Initialize(u16 arr, u16 psc, void (*updateCallBack_)(void));
extern void TIMER_Start(void);
extern void TIMER_Stop(void);

extern void TIMER_PWM_Initialize(u32 channel);
extern void TIMER_PWM_SetDuty(u32 channel, u32 ccr);
	
extern void TIMER_Test();
	
#ifdef __cplusplus 
};
#endif

#endif // _TIMER_h_
