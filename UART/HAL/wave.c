#include "spi.h"
#include <stm32f10x_spi.h>

/********正弦波输出表***********/
void SineWave_Data( u16 cycle ,u16 *D)
{
    u16 i;
    for( i=0;i<cycle;i++)
    {
        D[i]=(u16)((Um*sin(( 1.0*i/(cycle-1))*2*PI)+Um)*4095/3.3);
    }
}
  
/******************正弦波形表***********************/
#ifdef  Sine_WaveOutput_Enable 
     u16 SineWave_Value[256];       //用函数封装
#endif
 
 
/******DAC寄存器地址声明*******/  
#define DAC_DHR12R1    (u32)&(DAC->DHR12R1)   //DAC通道1输出地址
#define DAC_DHR12R2    (u32)&(DAC->DHR12R2)   //DAC通道2输出地址
/****************初始化引脚******************/
void SineWave_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开时钟
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //输出速率 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ; //选择引脚
    GPIO_SetBits(GPIOA,GPIO_Pin_5)  ;   //拉高输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化
}

/******************DAC初始化¯*************************/
void SineWave_DAC_Config( void)
{
    DAC_InitTypeDef            DAC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//开DAC时钟
     
  /**************DAC结构初始化*******************/
    DAC_StructInit(&DAC_InitStructure);    
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//不产生波形
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //不使能输出缓存
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;//DAC触发为定时器2触发
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);//初始化
  
    DAC_Cmd(DAC_Channel_1, ENABLE);    //使能DAC的通道1
    DAC_DMACmd(DAC_Channel_1, ENABLE); //使能DAC通道1的DMA  
}

/*********定时器初始化************/
void SineWave_TIM_Config(u32 Wave1_Fre)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//开时钟
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;     //不预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; //不分频
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
    TIM_TimeBaseStructure.TIM_Period = Wave1_Fre;//设置输出频率
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//设置TIME输出触发为更新模式
}

/*********DMA配置***********/
void SineWave_DMA_Config(void)
{                  
    DMA_InitTypeDef            DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//开启DMA2时钟
     
    DMA_StructInit( &DMA_InitStructure);        //DMA结构体初始化
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//从寄存器读数据
    DMA_InitStructure.DMA_BufferSize = 256;//寄存器大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;/优先级非常高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//关闭内存到内存模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环发送模式
		DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;//外设地址为DAC通道1的地址
　　　DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SineWave_Value;//波形数据表内存地址

　　　DMA_Init(DMA2_Channel3, &DMA_InitStructure);//初始化
　　　DMA_Cmd(DMA2_Channel3, ENABLE); //使能DMA通道3  　　            
}

void SineWave_Init(u16 Wave1_Fre)
{
　　u16 f1=(u16)(72000000/sizeof(SineWave_Value)*2/Wave1_Fre);//计算频率
  SineWave_Data( 256 ,SineWave_Value);     //生成输出正弦波的波形表
　　SineWave_GPIO_Config();             //初始化io
　　SineWave_TIM_Config(f1);            //初始化定时器
　　SineWave_DAC_Config();              //配置DAC
　　SineWave_DMA_Config();              //配置DMA
　　TIM_Cmd(TIM2, ENABLE);             //开启定时器
}