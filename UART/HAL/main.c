#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

#include <stm32f10x_rcc.h>
#include <stm32f10x_flash.h>
void RCCConfiguration()
{
	ErrorStatus HSEStartUpStatus;
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus==SUCCESS)
	{
		// set PLL multiplier
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		// set AHB CLock
		RCC_HCLKConfig(RCC_SYSCLK_Div1);	
		// setting APB1 Low Speed AHB(PCLK1), /2
		RCC_PCLK1Config(RCC_HCLK_Div2);
		// setting APB2 sHigh Speed AHB(PCLK2), /1
		RCC_PCLK2Config(RCC_HCLK_Div1);
		
		// FLASH wait timing
		// 0-24Mhz Latency_0
		//24-48Mhz Latency_1
		//48-72Mhz Latency_2
		FLASH_SetLatency(FLASH_Latency_2);
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // Flash Prefetch
		
		// Enabled PLL
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08);
	}
}

////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stm32f10x_usart.h>

void USART1_Init(int baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50Mhz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);	
}

#define USART1_REC_LEN 255
u8 USART1_RX_BUF[USART1_REC_LEN];
u16 USART1_RX_STA = 0;

void USART1_printf(char *fmt, ...)
{
	char buffer[USART1_REC_LEN+1];
	u8 i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);

	vsnprintf(buffer, USART1_REC_LEN+1, fmt, arg_ptr);
	while( (i<USART1_REC_LEN) && (i<strlen(buffer)) )
	{
		USART_SendData(USART1, (u8)buffer[i++]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	}
	va_end(arg_ptr);
}

void USART1_IRQHandler(void)
{
	u8 a;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		a = USART_ReceiveData(USART1);
		USART1_printf("Received 0x%02x, But Fuck you\r\n", a);
	}
}

void testUART1()
{
	u8 a=0;
	RCCConfiguration();
	delay_init(72);
	USART1_Init(115200);

	while (1)
	{
		// method 1
		// USART_SendData(USART1, 0x55);
		// while( USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET );

		// method 2
		// printf("2: Fuck 0x%02x\r\n", a++);
		
		// method 3
		USART1_printf("3: Fuck 0x%02x\r\n", a++);
		
		delay_ms(100);
	};
}

/////////////////////////////////////////////////////////////////
#include <stm32f10x_i2c.h>

#define I2CPORT GPIOB
#define I2C_SCL GPIO_Pin_6
#define I2C_SDA GPIO_Pin_7

#define HostAddress 0xc0
#define BusSpeed 200000

void I2C_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(I2CPORT, &GPIO_InitStructure);
}

void I2C_Config()
{
	I2C_InitTypeDef I2C_InitStructure;
	I2C_GPIO_Init();
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = HostAddress;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = BusSpeed;

	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
}

void I2C_Send_Buffer(u8 slaveAddr, u8 writeAddr, u8* buffer, u16 length)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(I2C1, writeAddr);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	while(length--)
	{
		I2C_SendData(I2C1, *buffer);
		buffer++;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_Send_Byte(u8 slaveAddr, u8 writeAddr, u8 buffer)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(I2C1, writeAddr);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_SendData(I2C1, buffer);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_Read_Buffer(u8 slaveAddr, u8 readAddr, u8* buffer, u16 length)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_Cmd(I2C1, ENABLE);
	I2C_SendData(I2C1, readAddr);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(length)
	{
		if(length==1)
		{
			I2C_AcknowledgeConfig(I2C1, DISABLE);
			I2C_GenerateSTOP(I2C1, ENABLE);
		}
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			*buffer = I2C_ReceiveData(I2C1);
			buffer++;
			length--;
		}
	};
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

u8 I2C_Read_Byte(u8 slaveAddr, u8 readAddr)
{
	u8 a;

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_Cmd(I2C1, ENABLE);
	I2C_SendData(I2C1, readAddr);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, slaveAddr, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	a = I2C_ReceiveData(I2C1);
	return a;
}

void testI2C()
{
	u8 a=0;
	RCCConfiguration();
	delay_init(72);
	I2C_Config();
	
	while (1)
	{
		I2C_Send_Byte(0x01, 0x00, a);
		
		a++;
		delay_ms(1000);
	};
}

/////////////////////////////////////////////////////////////
#include <stm32f10x_spi.h>

#define SPI2_PORT GPIOB
#define SPI2_MOSI GPIO_Pin_15
#define SPI2_MISO GPIO_Pin_14
#define SPI2_SCK 	GPIO_Pin_13
#define SPI2_NSS  GPIO_Pin_12

void SPI2_Init()
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI2_MISO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI2_MOSI | SPI2_SCK;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI2_NSS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI2_PORT, &GPIO_InitStructure);

	SPI_Cmd(SPI2, DISABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}

u8 SPI2_Send_Byte(u8 data)
{
	// wait for tx buffer empty
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);	
	SPI_I2S_SendData(SPI2, data);
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(SPI2);
}

void testSPI()
{
	RCCConfiguration();
	delay_init(72);
	
	SPI2_Init();
	while (1)
	{
		GPIO_ResetBits(SPI2_PORT, SPI2_NSS);
		
		SPI2_Send_Byte(0xCE);
		SPI2_Send_Byte(0x40);
		
		GPIO_SetBits(SPI2_PORT, SPI2_NSS);

		delay_ms(10);
		
		GPIO_ResetBits(SPI2_PORT, SPI2_NSS);
		
		SPI2_Send_Byte(0xBC);
		SPI2_Send_Byte(0xFC);
		
		GPIO_SetBits(SPI2_PORT, SPI2_NSS);

		delay_ms(10);
	};
}
	
int main(void)
{
	//testUART1();
	
	//testI2C();
	
	testSPI();
	
	return 0;
}
