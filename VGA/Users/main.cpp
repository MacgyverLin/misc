#include "usart.h"
#include "delay.h"

void initialize()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_15;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStructure);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	//GPIO_InitStructure.GPIO_Pin = A15 | A14 | A13 | A12 | A11 | A10 | A9 | A8 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7 | 
								  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//GPIO_InitStructure.GPIO_Pin = WE | OE | IO7 | IO6 | IO5 | IO4 | IO3 | IO2 | IO1 | IO0;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_3  | GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_6  | GPIO_Pin_7 | 
								  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART1_Init(115200);
	//USART1_printf(char *fmt, ...);
}

/*
    Horizontal timing
    -----------------
    
    Timer 1 period = 35156 Hz
    
    Timer 1 channel 1 generates a 2 us pulse for HSYNC each 28.4 us. : D = ( 2 / 28.4 )
    28.4 us    = Visible area + Front porch + Sync pulse + Back porch.
    HSYNC is 2 us long, so the math to do is:
    2us / 0,0138us = 144 system ticks.
    
    Timer 1 channel 2 generates a pulse equal to HSYNC + back porch = 2 + 3.55 = 5.55
    This interrupt will fire the DMA request to draw on the screen if vflag == 1.
    Since firing the DMA takes more or less 800ns, we'll add some extra time.
    The math for HSYNC + back porch is:
    (2us + 3,55us - dma) / 0,0138us = +-350 system ticks

    Horizontal timing info 800 + 24+72+128 = 1024
    --------------------------------------------

                    Dots       us
    --------------------------------------------        
    Sync pulse       72        2
	Front porch      24        0.66666666666667	
    Visible area    800        22.222222222222
    Back porch      128        3.5555555555556
    Whole line     1024        28.444444444444
*/

#include <stdlib.h>
#include <string.h>
#include <timer.h>

#define HSYNC				GPIO_Pin_0
#define VSYNC				GPIO_Pin_1
#define RED					GPIO_Pin_2
#define GREEN				GPIO_Pin_3
#define BLUE				GPIO_Pin_4
#define TESTPIN				GPIO_Pin_5

#define BPP_1
//#define BPP_2
//#define BPP_4
//#define BPP_8
//#define BPP_16
//#define BPP_32
#ifdef BPP_1
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define FRAME_BUFFER_SIZE ((SCREEN_WIDTH*SCREEN_HEIGHT)>>3)
#elif defined(BPP_2)
#define SCREEN_WIDTH 220
#define SCREEN_HEIGHT 148
#define FRAME_BUFFER_SIZE ((SCREEN_WIDTH*SCREEN_HEIGHT)>>2)
#elif defined(BPP_4)
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 100
#define FRAME_BUFFER_SIZE ((SCREEN_WIDTH*SCREEN_HEIGHT)>>1)
#elif defined(BPP_8)
#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 80
#define FRAME_BUFFER_SIZE ((SCREEN_WIDTH*SCREEN_HEIGHT))
#endif

typedef enum
{
	INITIAL = 0,
	HSYNC_PULSE,
	FRONT_PORCH,
	VISIBLE,
	BACK_PORCH,
	VSYNC_PULSE
}VGAState;

typedef struct
{
	u8* buffer;
	u8 counter;
	VGAState state;
}VGA;

VGA* current = 0;
void VGA_UpdateCallBack(void);

VGA* VGA_Create()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	VGA* vga = (VGA*)malloc(sizeof(VGA));
	if(!vga)
		return 0;
	memset(vga, 0, sizeof(VGA));
	
	vga->buffer = (u8*)malloc(FRAME_BUFFER_SIZE);
	memset(vga->buffer, 0, FRAME_BUFFER_SIZE);
	if(!vga->buffer)
	{
		free(vga);
		return 0;
	}	
	vga->counter = 0;
	vga->state = INITIAL;
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//GPIO_InitStructure.GPIO_Pin = A15 | A14 | A13 | A12 | A11 | A10 | A9 | A8 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0;
	GPIO_InitStructure.GPIO_Pin = HSYNC | VSYNC | RED  | GREEN  | BLUE | TESTPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	GPIO_ResetBits(GPIOA, HSYNC);
	GPIO_ResetBits(GPIOA, VSYNC);
	GPIO_ResetBits(GPIOA, RED);
	GPIO_ResetBits(GPIOA, GREEN);
	GPIO_ResetBits(GPIOA, BLUE);
	GPIO_ResetBits(GPIOA, TESTPIN);

	TIMER_Initialize(72, 0); // every 1us
	TIMER_SetCallBack(0, VGA_UpdateCallBack);
	TIMER_Stop();
	
	return vga;
}

void VGA_Destroy(VGA* vga)
{
	if(vga)
	{
		if(current==vga)
		{
			current = 0;
		}
		if(vga->buffer)
		{
			free(vga->buffer);
			vga->buffer = 0;
		}
		free(vga);
	}
}

void VGA_MakeCurrent(VGA* vga)
{
	current = vga;
}

void VGA_SetState(VGA* vga, VGAState state)
{
	switch(state)
	{
		case HSYNC_PULSE:
			GPIO_SetBits(GPIOA, HSYNC);
		
			vga->counter = 0;
			vga->state = HSYNC_PULSE;
			break;
		case FRONT_PORCH:
			GPIO_ResetBits(GPIOA, HSYNC);
		
			GPIO_ResetBits(GPIOA, RED); // black
			GPIO_ResetBits(GPIOA, GREEN);
			GPIO_ResetBits(GPIOA, BLUE);
			
			vga->counter = 0;
			vga->state = FRONT_PORCH;
			break;
		case VISIBLE:
			vga->counter = 0;
			vga->state = VISIBLE;
			break;
		case BACK_PORCH:
			GPIO_ResetBits(GPIOA, RED); // black
			GPIO_ResetBits(GPIOA, GREEN);
			GPIO_ResetBits(GPIOA, BLUE);
		
			vga->counter = 0;
			vga->state = BACK_PORCH;
			break;
		case INITIAL:
			vga->counter = 0;
			vga->state = INITIAL;
			break;		
	};
}

void VGA_UpdateState(VGA* vga)
{
	u8 test;
	
	if(GPIOA->ODR & GPIO_Pin_5)
	{
		GPIOA->BRR = GPIO_Pin_5;
	}
	else
	{
		GPIOA->BSRR = GPIO_Pin_5;
	}	
	return;
	
	switch(vga->state)
	{
		case HSYNC_PULSE:
			if(vga->counter++>=2)
			{
				// VGA_SetState(vga, FRONT_PORCH);
				GPIO_ResetBits(GPIOA, HSYNC);
		
				GPIO_ResetBits(GPIOA, RED); // black
				GPIO_ResetBits(GPIOA, GREEN);
				GPIO_ResetBits(GPIOA, BLUE);
			
				vga->counter = 0;
				vga->state = FRONT_PORCH;			
			}
			break;
		case FRONT_PORCH:
			if(vga->counter++>=2)
			{
				//VGA_SetState(vga, VISIBLE);
				vga->counter = 0;
				vga->state = VISIBLE;				
			}			
			break;
		case VISIBLE:
			if(vga->counter++>=22)
			{
				//VGA_SetState(vga, BACK_PORCH);
				GPIO_ResetBits(GPIOA, RED); // black
				GPIO_ResetBits(GPIOA, GREEN);
				GPIO_ResetBits(GPIOA, BLUE);
		
				vga->counter = 0;
				vga->state = BACK_PORCH;				
			}
			else
			{
				test = vga->counter & 0x07;
				if(test & 0x01) GPIOA->BSRR = RED;   else GPIOA->BRR = RED; 
				if(test & 0x02) GPIOA->BSRR = GREEN; else GPIOA->BRR = GREEN; 
				if(test & 0x04) GPIOA->BSRR = BLUE;  else GPIOA->BRR = BLUE; 
			}
			break;
		case BACK_PORCH:
			if(vga->counter++>=2)
			{
				//VGA_SetState(vga, HSYNC_PULSE);
				GPIO_SetBits(GPIOA, HSYNC);
		
				vga->counter = 0;
				vga->state = HSYNC_PULSE;				
			}			
			break;
		case INITIAL:
			{
				//VGA_SetState(vga, HSYNC_PULSE);
				GPIO_SetBits(GPIOA, HSYNC);
		
				vga->counter = 0;
				vga->state = HSYNC_PULSE;				
			}		
			break;		
	};
}

void VGA_UpdateCallBack(void)
{
	if(current)
	{
		VGA_UpdateState(current);
	}
}

void VGA_SetPixel(VGA* vga, u32 x, u32 y, u8 data)
{
#ifdef BPP_1
	u32 pos = (x * y);
	u32 idx = (pos >> 3);
	u32 mod = (pos & 0x07);
	
	u8* p = &vga->buffer[idx];
	*p &= ~(1<<mod);
	*p |= (data & 0x01) << mod;
#elif defined(BPP_2)
#elif defined(BPP_4)
#elif defined(BPP_8)
#endif
}

void VGA_StartDisplay(VGA* vga)
{
	TIMER_Start();
}

void VGA_StopDisplay(VGA* vga)
{
	TIMER_Stop();
}

void testVGA()
{
	u32 x;
	u32 y;
	VGA* vga = VGA_Create();
	VGA_MakeCurrent(vga);
	VGA_StartDisplay(vga);
	//VGA_StopDisplay(vga);
	
	if(vga)
	{
		for(y=0; y<SCREEN_HEIGHT; y++)
		{
			for(x=0; x<SCREEN_WIDTH; x++)
			{
				VGA_SetPixel(vga, x, y, x%2);
			}
		}
		while(1);
	
		VGA_Destroy(vga);
	}
}

#include <stm32f10x_rcc.h>

void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;

	RCC_DeInit();
   
	RCC_HSEConfig(RCC_HSE_ON);
   
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div1);
		//FLASH_SetLatency(FLASH_Latency_2);
		//FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while(RCC_GetSYSCLKSource() != 0x08);
	}
}

int main()
{
	//RCC_Configuration();
	
	delay_init(72);
	
	initialize();
	
	testVGA();
	
	return -1;
}
