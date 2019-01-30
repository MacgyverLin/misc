#ifndef _LCD_h_
#define _LCD_h_

#include "stm32f10x.h"
#include "vgmdef.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef LCD_TK022F2218
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#else
#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 800
#endif

void LCD_Initialize(void);
void LCD_Test(void);

void LCD_DrawRect888(u32 x, u32 y, u32 w, u32 h, u8 r, u8 g, u8 b);
void LCD_DrawRect565(u32 x, u32 y, u32 w, u32 h, u8 r, u8 g, u8 b);
void LCD_DrawRect444(u32 x, u32 y, u32 w, u32 h, u8 r, u8 g, u8 b);
void LCD_DrawImage888(u32 x, u32 y, const char* filename);
void LCD_DrawImage565(u32 x, u32 y, const char* filename);
void LCD_DrawImage444(u32 x, u32 y, const char* filename);
	
#define LCD_TK022F2218

//#define COLOR888	
#define COLOR565
//#define COLOR444
	
#define RGB888(r, g, b) ((r<<16) | (g<<8) | (b))
#define RGB565(r, g, b) (((((u32)r)<<8)&0xf800) | ((((u32)g)<<3)&0x07e0) | ((((u32)b)>>3)&0x001f))
#define RGB444(r, g, b) (((((u32)r)<<4)&0x0f00) | ((((u32)g)   )&0x00f0) | ((((u32)b)>>4)&0x000f))

#ifdef COLOR888
#define LCD_DrawRect LCD_DrawRect888
#define LCD_DrawImage LCD_DrawImage888
#endif
#ifdef COLOR565
#define LCD_DrawRect LCD_DrawRect565
#define LCD_DrawImage LCD_DrawImage565
#endif
#ifdef COLOR444
#define LCD_DrawRect LCD_DrawRect444
#define LCD_DrawImage LCD_DrawImage444
#endif

#ifdef __cplusplus
};
#endif

#endif
