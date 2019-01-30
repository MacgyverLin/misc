#ifndef _YM2612_H_
#define _YM2612_H_

#include "vgmdef.h"

#ifdef __cplusplus
extern "C" {
#endif

	s32 YM2612_Initialize(u8 chipID, u32 clock, u32 sampleRate);
	void YM2612_Shutdown(u8 chipID);
	void YM2612_Reset(u8 chipID);
	void YM2612_WriteRegister(u8 chipID, u32 address, u8 data);
	u8 YM2612_ReadStatus(u8 chipID);
	void YM2612_Update(u8 chipID, s32 **buffers, u32 length);

#ifdef __cplusplus
};
#endif

#endif
