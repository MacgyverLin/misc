#include "timer.h"
#include <stm32f10x_tim.h>
#include <stm32f10x_gpio.h>
#include <misc.h>

void (*timer3UpdateCallBack)(void) = 0;
void TIM3_IT_Update_CallBack(void)
{
	if(timer3UpdateCallBack)
	{
		timer3UpdateCallBack();
	}
}

void TIM3_IRQHandler( void )
{
	//if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
	{
	if(GPIOA->ODR & GPIO_Pin_5)
	{
		GPIOA->BRR = GPIO_Pin_5;
	}
	else
	{
		GPIOA->BSRR = GPIO_Pin_5;
	}	
/*	
		if(timer3UpdateCallBack)
		{
			timer3UpdateCallBack();
		}
	*/
	TIM3->SR = (uint16_t)~TIM_IT_Update;
		//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}	
}

void (*timer3CC1CallBack)(void) = 0;
void TIM3_IT_CC1_CallBack(void)
{
	if(timer3CC1CallBack)
	{
		timer3CC1CallBack();
	}
}

void (*timer3CC2CallBack)(void) = 0;
void TIM3_IT_CC2_CallBack(void)
{
	if(timer3CC2CallBack)
	{
		timer3CC2CallBack();
	}
}

void (*timer3CC3CallBack)(void) = 0;
void TIM3_IT_CC3_CallBack(void)
{
	if(timer3CC3CallBack)
	{
		timer3CC3CallBack();
	}
}

void (*timer3CC4CallBack)(void) = 0;
void TIM3_IT_CC4_CallBack(void)
{
	if(timer3CC4CallBack)
	{
		timer3CC4CallBack();
	}
}

void TIMER_Initialize(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;	
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  ENABLE);
	
    TIM_TimeBaseInitStructure.TIM_Period	    = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler	    = psc;
    TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIMER_Start()
{
	TIM_Cmd(TIM3, ENABLE);
}

void TIMER_Stop()
{
	TIM_Cmd(TIM3, DISABLE);
}

void TIMER_PWM_Initialize(u32 channel)
{
    GPIO_InitTypeDef 			GPIO_InitStructure;
    TIM_OCInitTypeDef     		TIM_OCInitStructure;
    
	/////////////////////////////////////////////////////////////
	if(channel==1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if(channel==2)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}	
	else if(channel==3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	else if(channel==4)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}	

	/////////////////////////////////////////////////////////////
	TIM_OCInitStructure.TIM_OCMode			=	TIM_OCMode_PWM1;        // CNT<CCR active
	TIM_OCInitStructure.TIM_OutputState		=	TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity		=	TIM_OCPolarity_High;	// active is High
	if(channel==1)
	{
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
	else if(channel==2)
	{
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
	else if(channel==3)
	{
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
	else if(channel==4)
	{
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}	
}

void TIMER_SetCallBack(u32 channel, void (*cb)(void))
{
	switch(channel)
	{
		default:
		case 0:
			if(cb)
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_Update);
				TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		
				timer3UpdateCallBack = cb;
			}
			else
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_Update);
				TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
				
				timer3UpdateCallBack = 0;
			}
		break;
			
		case 1:
			if(cb)
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_CC1);
				TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
		
				timer3CC1CallBack = cb;
			}
			else
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_CC1);
				TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
				
				timer3CC1CallBack = 0;
			}
		break;
		case 2:
			if(cb)
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_CC2);
				TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
		
				timer3CC2CallBack = cb;
			}
			else
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_CC2);
				TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);
				
				timer3CC2CallBack = 0;
			}
		break;
		case 3:
			if(cb)
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_CC3);
				TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
		
				timer3CC3CallBack = cb;
			}
			else
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_CC3);
				TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);
				
				timer3CC3CallBack = 0;
			}		
		break;
		case 4:
			if(cb)
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_CC4);
				TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
		
				timer3CC4CallBack = cb;
			}
			else
			{
				TIM_ClearFlag(TIM3, TIM_FLAG_CC4);
				TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);
				
				timer3CC4CallBack = 0;
			}				
		break;
	};
}

void TIMER_PWM_SetDuty(u32 channel, u32 ccr)
{
	switch(channel)
	{
		default:
		case 1:
			TIM_SetCompare1(TIM3, ccr);
		break;
		case 2:
			TIM_SetCompare2(TIM3, ccr);
		break;
		case 3:
			TIM_SetCompare3(TIM3, ccr);
		break;
		case 4:
			TIM_SetCompare4(TIM3, ccr);		
		break;
	};
}

void TIMER_Test()
{
	s8 inc[4]= { 1, 1, 1, 1 };
    u16 value[4] = { 0, 100, 50, 75 };
	u32 i;
	while(1)
	{    
		delay_ms(1);
		
		for(i=0; i<4; i++)
		{
			value[i] += inc[i];
			if(value[i]<=0)
				inc[i] = 1;
			if(value[i]>=1024)
				inc[i] = -1;
			
			TIMER_PWM_SetDuty(i+1, value[i]);
		}
    }	
}
