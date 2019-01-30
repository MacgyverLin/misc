#include "timer.h"
#include <stm32f10x_tim.h>
#include <stm32f10x_gpio.h>
#include <misc.h>

void (*updateCallBack)(void) = 0;

int TIM3_IT_Update_CallBack_Interval;
void TIM3_IT_Update_CallBack(void)
{
	if(updateCallBack)
	{
		updateCallBack();
	}
}

void TIMER_Initialize(u16 arr, u16 psc, void (*updateCallBack_)(void))
{
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;	
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);	
	//TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);	
	
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

	updateCallBack = updateCallBack_;
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//TIM_ClearFlag(TIM3, TIM_FLAG_CC3);
	//TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
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