#include <usart.h>
#include <stdarg.h>
#include <stm32f10x_usart.h>

#define USART1_REC_LEN 256
u8 USART1_RX_BUF[USART1_REC_LEN];
u16 USART1_RX_STA = 0;

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
	
	//USART_ClearFlag(USART1, USART_FLAG_RXNE);
	//USART_ClearFlag(USART1, USART_FLAG_TXE);
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

	USART_Cmd(USART1, ENABLE);	
}

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

void USART1_Send_Buffer(u8* buffer, u32 length)
{
	u32 i = 0;
	while(i<length)
	{
		USART_SendData(USART1, buffer[i++]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	}
}

void USART1_Send_Byte(u8 data)
{
	USART_SendData(USART1, data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
}

void USART1_Read_Buffer(u8* buffer, u16 length)
{
	u32 i = 0;
	while(i<length)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
		buffer[i++] = USART_ReceiveData(USART1);
	}
}

u8 USART1_Read_Byte()
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
	return USART_ReceiveData(USART1);
}

void USART1_IRQHandler(void)
{
	u8 a;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		a = USART_ReceiveData(USART1);
		//USART1_printf("Received 0x%02x, But Fuck you\r\n", a);
	}
}
