#include <io.h>

void pinMode(const GPIO* gpio, int mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = gpio->pin;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	if(mode==0)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	else
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	RCC_APB2PeriphClockCmd(gpio->rcc, ENABLE);

	GPIO_Init(gpio->type, &GPIO_InitStructure);
}

// emulate 8051
const GPIO PORT_A0 = { GPIOA, GPIO_Pin_0 , RCC_APB2Periph_GPIOA };
const GPIO PORT_A1 = { GPIOA, GPIO_Pin_1 , RCC_APB2Periph_GPIOA };
const GPIO PORT_A2 = { GPIOA, GPIO_Pin_2 , RCC_APB2Periph_GPIOA };
const GPIO PORT_A3 = { GPIOA, GPIO_Pin_3 , RCC_APB2Periph_GPIOA };
const GPIO PORT_A4 = { GPIOA, GPIO_Pin_4 , RCC_APB2Periph_GPIOA };
const GPIO PORT_A5 = { GPIOA, GPIO_Pin_5 , RCC_APB2Periph_GPIOA };
const GPIO PORT_A6 = { GPIOA, GPIO_Pin_6 , RCC_APB2Periph_GPIOA };
const GPIO PORT_A7 = { GPIOA, GPIO_Pin_7 , RCC_APB2Periph_GPIOA };

const GPIO PORT_A8 = { GPIOA, GPIO_Pin_8 , RCC_APB2Periph_GPIOA };
const GPIO PORT_A9 = { GPIOA, GPIO_Pin_9 , RCC_APB2Periph_GPIOA };
const GPIO PORT_A10 = { GPIOA, GPIO_Pin_10, RCC_APB2Periph_GPIOA };
const GPIO PORT_A11 = { GPIOA, GPIO_Pin_11, RCC_APB2Periph_GPIOA };
const GPIO PORT_A12 = { GPIOA, GPIO_Pin_12, RCC_APB2Periph_GPIOA };
const GPIO PORT_A13 = { GPIOA, GPIO_Pin_13, RCC_APB2Periph_GPIOA };
const GPIO PORT_A14 = { GPIOA, GPIO_Pin_14, RCC_APB2Periph_GPIOA };
const GPIO PORT_A15 = { GPIOA, GPIO_Pin_15, RCC_APB2Periph_GPIOA };

const GPIO PORT_B0 = { GPIOB, GPIO_Pin_0 , RCC_APB2Periph_GPIOB };
const GPIO PORT_B1 = { GPIOB, GPIO_Pin_1 , RCC_APB2Periph_GPIOB };
const GPIO PORT_B2 = { GPIOB, GPIO_Pin_2 , RCC_APB2Periph_GPIOB };
const GPIO PORT_B3 = { GPIOB, GPIO_Pin_3 , RCC_APB2Periph_GPIOB };
const GPIO PORT_B4 = { GPIOB, GPIO_Pin_4 , RCC_APB2Periph_GPIOB };
const GPIO PORT_B5 = { GPIOB, GPIO_Pin_5 , RCC_APB2Periph_GPIOB };
const GPIO PORT_B6 = { GPIOB, GPIO_Pin_6 , RCC_APB2Periph_GPIOB };
const GPIO PORT_B7 = { GPIOB, GPIO_Pin_7 , RCC_APB2Periph_GPIOB };

const GPIO PORT_B8 = { GPIOB, GPIO_Pin_8 , RCC_APB2Periph_GPIOB };
const GPIO PORT_B9 = { GPIOB, GPIO_Pin_9 , RCC_APB2Periph_GPIOB };
const GPIO PORT_B10 = { GPIOB, GPIO_Pin_10, RCC_APB2Periph_GPIOB };
const GPIO PORT_B11 = { GPIOB, GPIO_Pin_11, RCC_APB2Periph_GPIOB };
const GPIO PORT_B12 = { GPIOB, GPIO_Pin_12, RCC_APB2Periph_GPIOB };
const GPIO PORT_B13 = { GPIOB, GPIO_Pin_13, RCC_APB2Periph_GPIOB };
const GPIO PORT_B14 = { GPIOB, GPIO_Pin_14, RCC_APB2Periph_GPIOB };
const GPIO PORT_B15 = { GPIOB, GPIO_Pin_15, RCC_APB2Periph_GPIOB };
