#ifndef _VGMBoard_h_
#define _VGMBoard_h_

#include "vgmdef.h"

#ifdef __cplusplus
extern "C" {
#endif
	
s32 VGMBoard_Initialize(u32 clock, u32 sampleRate);
void VGMBoard_Shutdown(void);
void VGMBoard_Reset(void);
void VGMBoard_Update(void);
	
void VGMBoard_WriteYM2151(u8 addr, u8 data);
void VGMBoard_WriteYM2612(u8 addr, u8 data);
void VGMBoard_WriteSN76489(u8 data);
u32 VGMBoard_UpdateSamples(u32 nnnn);
	
#ifdef __cplusplus
};
#endif

#endif
