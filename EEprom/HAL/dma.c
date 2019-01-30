#include "dma.h"
#include <stm32f10x_dma.h>

void NVIC_Initialize(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
}

u8 buffer[10] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 };

void DMA_Initialize()
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel2);   							// DMA Reset
	//DMA_DeInit(DMA1_Channel3);   							// DMA Reset
	//DMA_DeInit(DMA1_Channel6);   							// DMA Reset
	DMA_StructInit(&DMA_InitStructure); 			 		// initi default params

	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)(&TIM3->CCR3);
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)buffer;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralDST;	// peri as dst
	DMA_InitStructure.DMA_BufferSize			= 10;
	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable; // peri fix address
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;  	 // memory increase
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte; // one byte
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_PeripheralDataSize_Byte;   //??????
	
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;  // 
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_High;
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;   //
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	//DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	//DMA_Init(DMA1_Channel6, &DMA_InitStructure);

	DMA_ClearFlag(DMA1_IT_TC2/*DMA1_IT_HR2*//*DMA1_IT_TE2*/); // clear interrupt
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC/*DMA_IT_HT*//*DMA_IT_TE*/, ENABLE); // dma1.2 for timer3.cmpr3
	//DMA_ClearFlag(DMA1_IT_TC3/*DMA1_IT_HR3*//*DMA1_IT_TE3*/);
	//DMA_ITConfig(DMA1_Channel3, DMA_IT_TC/*DMA_IT_HT*//*DMA_IT_TE*/, ENABLE); // dma1.3 for timer3.cmpr3
	//DMA_ClearFlag(DMA1_IT_TC6/*DMA1_IT_HR6*//*DMA1_IT_TE6*/);
	//DMA_ITConfig(DMA1_Channel6, DMA_IT_TC/*DMA_IT_HT*//*DMA_IT_TE*/, ENABLE); // dma1.6 for timer3.cmpr3
	
	DMA_Cmd(DMA1_Channel2, ENABLE);
	//DMA_Cmd(DMA1_Channel3, ENABLE);
	//DMA_Cmd(DMA1_Channel6, ENABLE);
}