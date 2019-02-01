#include "stm32f10x_usart.h"
#include "stm32f10x.h"

u8 USART1_RX_BUF[21]; 
u8 USART1_RX_CNT=0;

typedef struct
{
	u32 port;
	u32 baud;
	
	u8* txBuf;
	u32 txBufSize;
	u32 txCount;
	u32 txCounter;
	
	u8* rxBuf;
	u32 rxBufSize;
	u32 rxCount;
	u32 rxCounter;
	
	void (*onTXFinished)(void *);
	void (*onRXFinished)(void *);
}USART;

USART *usart1 = 0;

void onTXFinished(void* usart)
{
}

void onRXFinished(void* usart)
{
}

USART* USART_Create(u32 port, u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	USART *usart = (USART*)malloc(sizeof(USART));
	if(!usart)
		return 0;
	memset(usart, 0, sizeof(USART));
	
	usart->txBufSize = 32;
	usart->txBuf = (u8*)malloc(usart->txBufSize);
	if(!usart->txBuf)
		return 0;	
	
	usart->rxBufSize = 32;
	usart->rxBuf = (u8*)malloc(usart->rxBufSize);
	if(!usart->rxBuf)
		return 0;	
	
	if(port==1)
	{
		usart1 = usart;
		usart->port = port;
		usart->baud = baud;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
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
		
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);
		USART_Cmd(USART1, ENABLE);
		
		usart->onTXFinished = onTXFinished;
		usart->onRXFinished = onRXFinished;
		
		return usart;
	}
	else if(port==2)
	{
	}
	else if(port==3)
	{
	}
}

void USART_Destroy(USART* usart)
{
	if(usart)
	{
		while(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET);
		
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
		USART_Cmd(USART1, DISABLE);
		
		if(usart->txBuf)
		{
			free(usart->txBuf);
		}
		if(usart->rxBuf)
		{
			free(usart->rxBuf);
		}		
		free(usart);
		usart1 = 0;
	}
}

void USART_SendBuffer(USART* usart, u8* buffer, u32 size)
{
	memcpy(usart->txBuf, buffer, size);
	usart->txCounter = 0;
	usart->txCount = size;	
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	USART_SendData(USART1, usart->txBuf[usart->txCounter++]);
}

s8 USART_SendFinished(USART* usart)
{
	return usart->txCounter==usart->txCount;
}

void USART_ReceiveBuffer(USART* usart, u32 size)
{
	memset(usart->rxBuf, 0, size);
	usart->rxCounter = 0;
	usart->rxCount = size;	
	
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

s8 USART_ReceiveFinished(USART* usart)
{
	return usart->rxCounter==usart->rxCount;
}

void USART1_IRQHandler(void) 
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		//assert(usart1->rxCounter < usart1->rxBufSize);
		usart1->rxBuf[usart1->rxCounter++] = USART_ReceiveData(USART1);
		if(usart1->rxCounter>=usart1->rxCount && usart1->onRXFinished)
		{
			usart1->onRXFinished(usart1);
			
			usart1->rxCounter = 0;
			usart1->rxCount = 0;
		}

		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET) 
	{
		//assert(usart1->txCounter < usart1->txBufSize);
		USART_SendData(USART1, usart1->txBuf[usart1->txCounter++]);
		if(usart1->txCounter>=usart1->txCount && usart1->onTXFinished)
		{
			usart1->onTXFinished(usart1);
			
			usart1->txCounter = 0;
			usart1->txCount = 0;
		}
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
}

void Usart1_Init(u32 baud)
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
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
	
	USART_ClearFlag(USART1,USART_IT_RXNE);
	USART_ClearFlag(USART1,USART_IT_TC);
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
    //USART_ITConfig(USART1, USART_IT_TC, ENABLE);	
    USART_Cmd(USART1, ENABLE);
}

void USART1_SendData(u8 *buf, u16 len)
{
    u16 t;
    for(t=0; t<len; t++)
    {           
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
        USART_SendData(USART1, buf[t]); 
    }     
}

void USART1_ReceiveData(u8 *buf, u16 len)
{
    u16 t;
    for(t=0; t<len; t++)
    {           
        while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
        buf[t] = USART_ReceiveData(USART1);
    }     
}

int getchar(void)
{
	u8 c;
	USART1_ReceiveData(&c, 1);
	
	return c;
}

char *gets(char *str)
{
	u32 i = 0;
	u32 r;
	do
	{
		r = getchar();
		if(r=='\n' || r==-1)
		{
			str[i] = 0;
		}
		else
		{
			str[i] = r;
		}
	}while(str[i++]==0);
	
	return str; 
}

int putchar(int c)
{
	USART1_SendData((u8*)&c, 1);
	
	return c;
}

int puts(const char *str)
{
	u32 i;
	u32 length = strlen(str);
	for(i=0; i<length; i++)
	{
		putchar(str[i]);
	}
}

int testUSARTSync()
{
	char buffer[256];	
	u32 counter = 0;
    Usart1_Init(57600);
	
    while(1)
    {
        int c = getchar();
		putchar(c);
		//sprintf(buffer, "Counter=%d\n", counter++);
		//puts(buffer);
		//puts(gets(buffer));
    };
 	
	return 0;
}

const char* msg = "I am a good boy.\n";

int testUSARTASync()
{
	USART* usart = USART_Create(1, 57600);
	if(usart)
	{
		while(1)
		{
			if(USART_SendFinished(usart))
				USART_SendBuffer(usart, (u8*)msg, strlen(msg)+1);
			if(USART_ReceiveFinished(usart))
				USART_ReceiveBuffer(usart, strlen(msg)+1);			
		};
		USART_Destroy(usart);
		usart = 0;
	}
}

int main(void)
{
	//return testUSARTSync();
	return testUSARTASync();
}
