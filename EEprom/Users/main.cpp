#include "io.h"
#include "usart.h"
#include "delay.h"

void initialize()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_15;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	IO_MODE(A15	, 0);
	IO_MODE(A12	, 0);
	IO_MODE(A7	, 0);
	IO_MODE(A6	, 0);
	IO_MODE(A5	, 0);
	IO_MODE(A4	, 0);
	IO_MODE(A3	, 0);
	IO_MODE(A2	, 0);
	IO_MODE(A1	, 0);
	IO_MODE(A0	, 0);
	IO_MODE(IO0	, 0);
	IO_MODE(IO1	, 0);
	IO_MODE(IO2	, 0);

	IO_MODE(WE	, 0);
	IO_MODE(A14	, 0);
	IO_MODE(A13	, 0);
	IO_MODE(A8	, 0);
	IO_MODE(A9	, 0);
	IO_MODE(A11 , 0);
	IO_MODE(OE	, 0);
	IO_MODE(A10	, 0);
	IO_MODE(IO7 , 0);
	IO_MODE(IO6	, 0);
	IO_MODE(IO5	, 0);
	IO_MODE(IO4	, 0);
	IO_MODE(IO3	, 0);

	USART1_Init(115200);
	//USART1_printf(char *fmt, ...);
}

void setAddress(u16 address)
{
	IO_WRITE( A0, address & 0x0001);
	IO_WRITE( A1, address & 0x0002);
	IO_WRITE( A2, address & 0x0004);
	IO_WRITE( A3, address & 0x0008);
	IO_WRITE( A4, address & 0x0010);
	IO_WRITE( A5, address & 0x0020);
	IO_WRITE( A6, address & 0x0040);
	IO_WRITE( A7, address & 0x0080);
	
	IO_WRITE( A8, address & 0x0100);
	IO_WRITE( A9, address & 0x0200);
	IO_WRITE(A10, address & 0x0400);
	IO_WRITE(A11, address & 0x0800);
	IO_WRITE(A12, address & 0x1000);
	IO_WRITE(A13, address & 0x2000);
	IO_WRITE(A14, address & 0x4000);
	IO_WRITE(A15, address & 0x8000);
}

void setData(u8 data)
{
	IO_WRITE(IO0, data & 0x0001);
	IO_WRITE(IO1, data & 0x0002);
	IO_WRITE(IO2, data & 0x0004);
	IO_WRITE(IO3, data & 0x0008);
	IO_WRITE(IO4, data & 0x0010);
	IO_WRITE(IO5, data & 0x0020);
	IO_WRITE(IO6, data & 0x0040);
	IO_WRITE(IO7, data & 0x0080);
}

u8 getData()
{
	u8 d = 0;
	d |= IO_READ(IO7)<<7;
	d |= IO_READ(IO6)<<6;
	d |= IO_READ(IO5)<<5;
	d |= IO_READ(IO4)<<4;
	d |= IO_READ(IO3)<<3;
	d |= IO_READ(IO2)<<2;
	d |= IO_READ(IO1)<<1;
	d |= IO_READ(IO0);
}

void beginWriteByte()
{
	IO_MODE(IO0, 0);
	IO_MODE(IO1, 0);
	IO_MODE(IO2, 0);
	IO_MODE(IO3, 0);
	IO_MODE(IO4, 0);
	IO_MODE(IO5, 0);
	IO_MODE(IO6, 0);
	IO_MODE(IO7, 0);
	
	IO_WRITE(OE, 1);
	IO_WRITE(WE, 1);
	delay_us(1);
}

void endWriteByte()
{
	IO_WRITE(OE, 1);
	IO_WRITE(WE, 1);
}

void writeByte(u16 address, u8 data)
{
	setAddress(address);
	setData(data);

	IO_WRITE(WE, 1);
	delay_us(1);
	IO_WRITE(WE, 0);
}

void beginReadByte()
{
	IO_MODE(IO0, 1);
	IO_MODE(IO1, 1);
	IO_MODE(IO2, 1);
	IO_MODE(IO3, 1);
	IO_MODE(IO4, 1);
	IO_MODE(IO5, 1);
	IO_MODE(IO6, 1);
	IO_MODE(IO7, 1);
	
	IO_WRITE(OE, 1);
	IO_WRITE(WE, 1);
	delay_us(1);
}

u8 readByte(u16 address)
{
	setAddress(address);
	// setData(data);
	delay_us(1);
	
	IO_WRITE(OE, 0);
	delay_us(1);

	IO_WRITE(OE, 1);
}

void endReadByte()
{
	IO_WRITE(OE, 1);
	IO_WRITE(WE, 1);
}

const char data[] = 
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
};

void testWriteEEprom()
{
	initialize();
	
	beginWriteByte();
	for(int i=0; i<sizeof(data)/sizeof(data[0]); i++)
	{
		writeByte(i, 0x00);
	}
	
	endWriteByte();
	
	beginReadByte();
	for(int i=0; i<sizeof(data)/sizeof(data[0]); i++)
	{
		u8 d = readByte(i);
		if(d!=data[i])
		{
			// printf("byte mismatch at %04x\n", i);
		}
	}
	
	endReadByte();

	while (1)
	{
	};	
}

void testVGA()
{
}

int main()
{
	// testWriteEEprom();
	
	testVGA();
	
	return -1;
}