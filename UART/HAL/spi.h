#ifndef _SPI_H_
#define _SPI_H_

#include "vgmdef.h"

#ifdef __cplusplus 
extern "C"
{
#endif
	
// SPI�����ٶ�����  
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 			4
#define SPI_SPEED_64 			5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7
						  	    													  
void SPI2_Init(void);			 //��ʼ��SPI��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�

#ifdef __cplusplus 
};
#endif
	
#endif
