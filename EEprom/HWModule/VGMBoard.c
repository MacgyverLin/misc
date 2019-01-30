#include "VGMBoard.h"
#include <delay.h>
#include <io.h>
#include <spi.h>
#include <malloc.h>
#include <mmc_sd.h>
#include <ff.h>

FATFS fs;
#define SAMPLE_PERIOD (22.67573f * 0.98)

u32 VGMBoard_UpdateSamples(u32 nnnn)
{
	u32 time = nnnn * SAMPLE_PERIOD;
	delay_us(time);
	
	return nnnn;
}

void VGMBoard_InitIO()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_15;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	IO_MODE(D0		, 0);
	IO_MODE(D1		, 0);
	IO_MODE(D2		, 0);
	IO_MODE(D3		, 0);
	IO_MODE(D4		, 0);
	IO_MODE(D5		, 0);
	IO_MODE(D6		, 0);
	IO_MODE(D7		, 0);
	IO_MODE(YM_CS	, 0);
	IO_MODE(WR		, 0);
	IO_MODE(A0		, 0);
	IO_MODE(A1		, 0);

	IO_MODE(SN_CLK_CS		, 0);
	IO_MODE(SN_WR				, 0);
	IO_MODE(RD					, 0);
	IO_MODE(LCD_CS			, 0);
	IO_MODE(SD_CS				, 0);
	IO_MODE(YM_CLK_CS		, 0);
	IO_MODE(VGM_CS			, 0);
	IO_MODE(BUTTON_MODE	, 1);
	IO_MODE(BUTTON_NEXT	, 1);
	IO_MODE(BUTTON_PREV	, 1);
	IO_MODE(RXD					, 0);
	IO_MODE(TXD					, 0);
	IO_MODE(BUTTON_SEL	, 1);
	//IO_MODE(SPI_MOSI		, 0);
	//IO_MODE(SPI_MISO		, 1);
	//IO_MODE(SPI_CLK			, 0);
	IO_MODE(RESET				, 0);	
	
	// Data
	IO_WRITE(D0, 0);
	IO_WRITE(D1, 0);
	IO_WRITE(D2, 0);
	IO_WRITE(D3, 0);
	IO_WRITE(D4, 0);
	IO_WRITE(D5, 0);
	IO_WRITE(D6, 0);
	IO_WRITE(D7, 0);	

	// YM2612
	IO_WRITE(YM_CLK_CS, 1);
	IO_WRITE(YM_CS, 1);
	IO_WRITE(WR, 1);
	IO_WRITE(A0, 1);
	IO_WRITE(A1, 1);
	IO_WRITE(RESET, 1);

	// SN76489
	IO_WRITE(SN_CLK_CS, 1);
	IO_WRITE(SN_WR, 1);

	// Misc
	IO_WRITE(VGM_CS			, 1);	
	IO_WRITE(RD					, 1);
	
	// LCD
	IO_WRITE(LCD_CS			, 1);
	
	// SD
	IO_WRITE(SD_CS			, 1);
	
	IO_WRITE(BUTTON_MODE, 1);
	IO_WRITE(BUTTON_NEXT, 1);
	IO_WRITE(BUTTON_PREV, 1);
	IO_WRITE(BUTTON_SEL	, 1);

	// UART
	IO_WRITE(RXD				, 1);
	IO_WRITE(TXD				, 1);
	
	// reset signal
	delay_us(100);
	IO_WRITE(RESET, 0);
	delay_us(100);
	IO_WRITE(RESET, 1);
	delay_us(100);
}

void VGMBoard_InitSPI2()
{
	SPI2_Init();
	SPI2_SetSpeed(SPI_SPEED_256);
}

#define TEST_YM2151
void VGMBoard_InitYMClock()
{
	IO_WRITE(YM_CLK_CS, 0);
	delay_us(10);

#ifdef TEST_YM2151
	SPI2_ReadWriteByte(0xBC);
	SPI2_ReadWriteByte(0xFC);	
#else
	SPI2_ReadWriteByte(0xCE);
	SPI2_ReadWriteByte(0x40);
#endif

	IO_WRITE(YM_CLK_CS, 1);
	delay_us(10);
}

void VGMBoard_InitSNClock()
{
	IO_WRITE(SN_CLK_CS, 0);
	delay_us(10);

	SPI2_ReadWriteByte(0xBC);
	SPI2_ReadWriteByte(0xFC);

	IO_WRITE(SN_CLK_CS, 1);
	delay_us(10);

	VGMBoard_WriteSN76489(0x9f);
	VGMBoard_WriteSN76489(0xbf);
	VGMBoard_WriteSN76489(0xdf);
	VGMBoard_WriteSN76489(0xff);
}

void VGMBoard_SetData(u8 data)
{
	IO_WRITE(D0, data & 0x01);
	IO_WRITE(D1, data & 0x02);
	IO_WRITE(D2, data & 0x04);
	IO_WRITE(D3, data & 0x08);
	IO_WRITE(D4, data & 0x10);
	IO_WRITE(D5, data & 0x20);
	IO_WRITE(D6, data & 0x40);
	IO_WRITE(D7, data & 0x80);
}

void VGMBoard_WriteYM2151(u8 addr, u8 data)
{
	/////////////////////////
	VGMBoard_SetData(addr);
	IO_WRITE(A0, 0);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(YM_CS, 0);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(WR, 0);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(WR, 1);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(YM_CS, 1);
	delay_us(DATA_SETUP_TIME);

	/////////////////////////
	VGMBoard_SetData(data);
	IO_WRITE(A0, 1);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(YM_CS, 0);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(WR, 0);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(WR, 1);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(YM_CS, 1);
	delay_us(DATA_SETUP_TIME);
}

void VGMBoard_WriteYM2612(u8 addr, u8 data)
{
	/////////////////////////
	VGMBoard_SetData(data);
	IO_WRITE(A0, addr & 0x01);
	IO_WRITE(A1, addr & 0x02);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(YM_CS, 0);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(WR, 0);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(WR, 1);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(YM_CS, 1);
	delay_us(DATA_SETUP_TIME);
}

void VGMBoard_WriteSN76489(u8 data)
{
	/////////////////////////
	VGMBoard_SetData(data);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(SN_WR, 0);
	delay_us(DATA_SETUP_TIME);

	IO_WRITE(SN_WR, 1);
	delay_us(DATA_SETUP_TIME);
} 

s32 VGMBoard_Initialize(u32 clock, u32 sampleRate)
{
	//SystemInit();
	delay_init(72);    //????? 

	VGMBoard_InitIO();
	VGMBoard_InitSPI2();
	VGMBoard_InitYMClock(); // generate 4.79Mhz
	VGMBoard_InitSNClock(); // generate 3.57Mhz	
	
	SPI2_SetSpeed(SPI_SPEED_2);
#ifdef USE_MYMEM
 	mem_init();	// initialize malloc
#else
#endif 	
	while(SD_Initialize()) // initialize SDCard
	{
			// USART1_printf("SD Card Error!");
			// USART1_printf("Please Check!");
			delay_ms(500);
	}
	
	f_mount(&fs, (const TCHAR*)"0:", 0);
	
	return -1;
}

void VGMBoard_Shutdown()
{
}

void VGMBoard_Reset()
{
}

void VGMBoard_Update()
{
	/*
	u8 a = IO_READ(NEXT_PAUSE);
	if(a)
	{
			//IO_WRITE(PWR_HOLD		, 1);
	}
	else
	{
			IO_MODE(PWR_HOLD		, 1);
	}
	*/
}
