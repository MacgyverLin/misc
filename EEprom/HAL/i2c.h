#ifndef _I2C_h_
#define _I2C_h_

#ifdef __cplusplus 
extern "C"
{
#endif
	
#include <stm32f10x.h>

extern void I2C_GPIO_Init(void);
extern void I2C_Config(void);
extern void I2C_Send_Buffer(u8 slaveAddr, u8 writeAddr, u8* buffer, u16 length);
extern void I2C_Send_Byte(u8 slaveAddr, u8 writeAddr, u8 buffer);
extern void I2C_Read_Buffer(u8 slaveAddr, u8 readAddr, u8* buffer, u16 length);
extern u8 I2C_Read_Byte(u8 slaveAddr, u8 readAddr);
	
#ifdef __cplusplus 
};
#endif

#endif // _I2C_h_
