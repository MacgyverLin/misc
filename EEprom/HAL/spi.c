#include "spi.h"
#include <stm32f10x_spi.h>

#define SPI2_PORT GPIOB
#define SPI2_MOSI GPIO_Pin_15
#define SPI2_MISO GPIO_Pin_14
#define SPI2_SCK  GPIO_Pin_13
#define SPI2_NSS  GPIO_Pin_12

void SPI2_Init(void)
{	
	/*
	RCC->APB2ENR|=1<<2;       //PORTA时钟使能 	 
	RCC->APB2ENR|=1<<12;      //SPI1时钟使能 
		   
	//这里只针对SPI口初始化
	GPIOA->CRL&=0X000FFFFF; 
	GPIOA->CRL|=0XBBB00000;//PA5.6.7复用 	    
	GPIOA->ODR|=0X7<<5;    //PA5.6.7上拉
		
	SPI1->CR1|=0<<10;//全双工模式	
	SPI1->CR1|=1<<9; //软件nss管理
	SPI1->CR1|=1<<8;  
	SPI1->CR1|=1<<2; //SPI主机
	SPI1->CR1|=0<<11;//8bit数据格式	
	SPI1->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
	SPI1->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1  
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI设备使能
	SPI2_ReadWriteByte(0xff);//启动传输(主要作用：维持MOSI为高)		 
	*/
	
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
	
	SPI2_ReadWriteByte(0xff);//启动传输(主要作用：维持MOSI为高)		 	
}   

// SPI1 速度设置函数
// SpeedSet:0~7
// SPI速度=fAPB2/2^(SpeedSet+1)
// APB2时钟一般为72Mhz
void SPI2_SetSpeed(u8 SpeedSet)
{
	SpeedSet &= 0X07;			//限制范围
	SPI2->CR1 &= 0XFFC7; 
	SPI2->CR1 |= SpeedSet<<3;	//设置SPI2速度  
	SPI2->CR1 |= 1<<6; 		//SPI设备使能 
} 

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 data)
{		
	/*
	u16 retry=0;				 
	while((SPI2->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}			  
	SPI2->DR = TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI2->SR & 1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}	  						    
	return SPI2->DR;          //返回收到的数据				    
	*/
	
	u16 retry=0;
	// wait for tx buffer empty
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET)
	{
		retry++;
		if(retry>0XFFFE)
			return 0;
	}
	SPI_I2S_SendData(SPI2, data);
	
	retry=0;
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET)
	{
		retry++;
		if(retry>0XFFFE)
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI2);	
}
