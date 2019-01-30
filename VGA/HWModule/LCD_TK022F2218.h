#ifndef _LCD_h_
#define _LCD_h_

#include "stm32f10x.h"
#include "vgmdef.h"

#ifdef __cplusplus
extern "C" {
#endif

void LCD_Initialize(void);
void LCD_DrawRect(u32 x, u32 y, u32 w, u32 h, u16 color);
void LCD_DrawBMP(u32 x, u32 y, const u8* filename);

#ifdef __cplusplus
};
#endif

#endif
