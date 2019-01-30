#include "JPEG.h"

u32 JPEG_readCB(void *ptr, s8 *buffer, u32 size)
{
	u32 br;
	JPEG* jpeg = (JPEG *)ptr;

#ifdef STM32
	f_read(&jpeg->fil, buffer, size, &br);
	return br;
#else
	return fread(buffer, 1, size, jpeg->fil);
#endif
}

u32 JPEG_writeCB(void *ptr, s8 *buffer, u32 size)
{
	JPEG* jpeg = (JPEG *)ptr;

	return 0;
}

u32 JPEG_seekCB(void *ptr, u32 where, u32 offset)
{
	JPEG* jpeg = (JPEG *)ptr;

	return 0;
}

JPEG* JPEG_open(const s8 *filename)
{
	JPEG* jpeg = (JPEG*)malloc(sizeof(JPEG));
	u32 res;

	/* Initialize the JPEG decompression object with default error handling. */
	jpeg->cinfo.err = jpeg_std_error(&jpeg->jerr);
	jpeg_create_decompress(&jpeg->cinfo);

	/* Add some application-specific error messages (from cderror.h) */

	jpeg->cinfo.dct_method = JDCT_IFAST;
	//jpeg->cinfo.dither_mode = JDITHER_FS;
	//jpeg->cinfo.dither_mode = JDITHER_NONE;
	//jpeg->cinfo.dither_mode = JDITHER_ORDERED;

	//jpeg->cinfo.out_color_space = JCS_GRAYSCALE;
	jpeg->cinfo.out_color_space = JCS_RGB;
	//jpeg->cinfo.out_color_space = JCS_YCbCr;
	//jpeg->cinfo.out_color_space = JCS_CMYK;
	//jpeg->cinfo.out_color_space = JCS_YCCK;
	//jpeg->cinfo.out_color_space = JCS_BG_RGB;
	//jpeg->cinfo.out_color_space = JCS_BG_YCC;
	jpeg->cinfo.mem->max_memory_to_use = 2 * 1000;

#ifdef STM32
	res = f_open(&jpeg->fil, filename, FA_OPEN_ALWAYS | FA_READ);
	if (res != FR_OK)
	{
		jpeg_destroy_decompress(&jpeg->cinfo);
		free(jpeg);

		return 0;
	}

	/* Specify data source for decompression */
	jpeg_stdio_src(&jpeg->cinfo, jpeg->fil);
#else
	jpeg->fil = fopen(filename, "rb");
	if (!jpeg->fil)
	{
		jpeg_destroy_decompress(&jpeg->cinfo);
		free(jpeg);

		return 0;
	}

	jpeg->userIO.ReadCB = JPEG_readCB;
	jpeg->userIO.WriteCB = JPEG_writeCB;
	jpeg->userIO.SeekCB = JPEG_seekCB;
	jpeg->userIO.ptr = jpeg;

	/* Specify data source for decompression */
	//jpeg_stdio_src(&jpeg->cinfo, jpeg->fil);
	jpeg_userio_src(&jpeg->cinfo, &jpeg->userIO);
#endif
	/* Read file header, set default decompression parameters */
	jpeg_read_header(&jpeg->cinfo, TRUE);

	jpeg_start_decompress(&jpeg->cinfo);

	return jpeg;
}

u32 JPEG_DecodeScanLine(JPEG* jpeg, s8 **buffer)
{
	int num_scanlines;
	int row_stride;

	if (!jpeg)
		return 0;

	if (jpeg->cinfo.output_scanline >= jpeg->cinfo.output_height)
		return 0;

	//FILE *fptr = fopen("1.raw", "wb");

	while (jpeg->cinfo.output_scanline < jpeg->cinfo.output_height)
	{
		num_scanlines = jpeg_read_scanlines(&jpeg->cinfo, buffer, 1);
		//fwrite(buffer[0], 1, jpeg->cinfo.image_width * 3, fptr);
	}

	//fclose(fptr);

	return -1;
}

void JPEG_close(JPEG* jpeg)
{
	if (jpeg)
	{
		jpeg_finish_decompress(&jpeg->cinfo);

#ifdef STM32
		f_close(&jpeg->fil);
#else
		fclose(jpeg->fil);
#endif
		jpeg_finish_decompress(&jpeg->cinfo);
		jpeg_destroy_decompress(&jpeg->cinfo);

		free(jpeg);
	}
}

/*
int test(int argc, char **argv)
{
	JPEG* jpeg = JPEG_open("1.jpg");

	char* buffer = (char*)malloc(jpeg->cinfo.output_components*jpeg->cinfo.output_width);
	JPEG_DecodeScanLine(jpeg, &buffer);
	free(buffer);

	JPEG_close(jpeg);
}
*/