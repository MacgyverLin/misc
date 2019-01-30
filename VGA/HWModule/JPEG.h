#ifndef _JPEG_h_
#define _JPEG_h_

#include "vgmdef.h"
#ifdef STM32
#include <ff.h>
#else
#include <stdio.h>
#endif
#include "jinclude.h"
#include "jpeglib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	UserIO userIO;
#ifdef STM32
	FIL fil;
#else
	FILE* fil;
#endif
}JPEG;

JPEG* JPEG_Open(const s8 *filename);
u32 JPEG_DecodeScanLine(JPEG* jpeg, s8 **buffer);
void JPEG_Close(JPEG* jpeg);

#ifdef __cplusplus
};
#endif

#endif