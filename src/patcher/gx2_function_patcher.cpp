/****************************************************************************
 * Copyright (C) 2017 Maschell, BullyWiiPlaza
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include <stdlib.h> // malloc()/free()
#include <gd.h> // image library
#include "pad_function_patcher.h"
#include "../utils/logger.h"

/*typedef struct _R8G8B8A8_COLOR {
	u8 R, G, B, A;
} R8G8B8A8_COLOR;

void UnormR8G8B8A8TogdImage(gdImagePtr *gdImgTmp, void *image_data, u32 width, u32 rows, u32 pitch) {
	*gdImgTmp = gdImageCreateTrueColor(width / 2, rows / 2);

	R8G8B8A8_COLOR *buffer = (R8G8B8A8_COLOR *) image_data;
	R8G8B8A8_COLOR val;
	for (u32 row = 0; row < rows; ++row) {
		for (u32 x = 0; x < width; ++x) {
			val = buffer[row * pitch + x];
			gdImageSetPixel(*gdImgTmp, x / 2, row / 2, gdTrueColor(val.R, val.G, val.B));
			++x;
		}
		++row;
	}
}

void UnormR8G8B8A82Yuv420p(u8 **destination_, void *image_data, int *dest_img_size, u32 width, u32 height, u32 pitch) {
	u32 image_size = width * height;
	u32 upos = image_size;
	u32 vpos = upos + upos / 4;
	*dest_img_size = (vpos + upos / 4);
	if (*destination_) {
		free(destination_);
	}
	*destination_ = (u8 *) malloc(sizeof(u8) * *dest_img_size);
	u8 *destination = *destination_;
	if (!destination) {
		*dest_img_size = 0;
		return;
	}
	log_printf("allocated %d \n", *dest_img_size);

	R8G8B8A8_COLOR *buffer = (R8G8B8A8_COLOR *) image_data;

	u32 i = 0;

	for (u32 line = 0; line < height; ++line) {
		if (!(line % 2)) {
			for (u32 x = 0; x < width; x += 2) {
				u8 r = buffer[line * pitch + x].R;
				u8 g = buffer[line * pitch + x].G;
				u8 b = buffer[line * pitch + x].B;

				destination[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;

				destination[upos++] = ((-38 * r + -74 * g + 112 * b) >> 8) + 128;
				destination[vpos++] = ((112 * r + -94 * g + -18 * b) >> 8) + 128;

				r = buffer[line * pitch + x].R;
				g = buffer[line * pitch + x].G;
				b = buffer[line * pitch + x].B;

				destination[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
			}
		} else {
			for (u32 x = 0; x < width; x += 1) {
				u8 r = buffer[line * pitch + x].R;
				u8 g = buffer[line * pitch + x].G;
				u8 b = buffer[line * pitch + x].B;

				destination[i++] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
			}
		}
	}
	sleep(1);
	log_printf("done %d \n", *dest_img_size);
}

void UnormR10G10B10A2TogdImage(gdImagePtr *gdImgTmp, void *image_data, u32 width, u32 rows, u32 pitch) {
	u32 *buffer = (u32 *) image_data;
	u32 val = 0;
	for (u32 row = 0; row < rows; ++row) {
		for (u32 x = 0; x < width; ++x) {

				R ((test >> 24) & 0xFF))
				G ((test >> 14) & 0xFF))
				B ((test >> 4) & 0xFF))
				alpha (test & 0x3);

			val = buffer[row * pitch + x];
			gdImageSetPixel(*gdImgTmp, x, row, gdTrueColor(((val >> 24) & 0xFF),
														   ((val >> 14) & 0xFF),
														   ((val >> 4) & 0xFF)));
		}
	}
}

typedef struct
{
	u32 img_size;
	u32 img_id;
	void * img_data;
} JpegData;*/

static volatile int executionCounter = 0;

declareFunctionHook(void, GX2CopyColorBufferToScanBuffer, const GX2ColorBuffer *colorBuffer, s32 scan_target) {
	if (executionCounter > 120) {
		GX2Surface surface = colorBuffer->surface;
		/*s32 format = surface.format;

		gdImagePtr gdImagePtr = 0;
		bool no_convert;
		u8 *image_data = NULL;
		int img_size = 0;

		if (format == 0x1A) {
			UnormR8G8B8A82Yuv420p(&image_data, surface.image_data, &img_size, surface.width, surface.height,
								  surface.pitch);
		} else if (format == 0x19) {
			no_convert = true;
			UnormR8G8B8A8TogdImage(&gdImagePtr, surface.image_data, surface.width, surface.height, surface.pitch);
		} else {
			no_convert = true;
		}

		int imd_size = 0;
		void *data = gdImageJpegPtr(gdImagePtr, &imd_size, 95);
		if (data) {
			JpegData img;
			img.img_size = imd_size;
			img.img_data = data;
			img.img_id = 0;
		}*/

		log_printf("GX2CopyColorBufferToScanBuffer {surface width:%d, height:%d, image size:%d, image data:%x}\n", surface.width, surface.height, surface.image_size, surface.image_data);

		executionCounter = 0;
	}

	executionCounter++;

	real_GX2CopyColorBufferToScanBuffer(colorBuffer, scan_target);
}

FunctionHook method_hooks_gx2[] __attribute__((section(".data"))) = {
		makeFunctionHook(GX2CopyColorBufferToScanBuffer, LIB_GX2, STATIC_FUNCTION)
};

u32 method_hooks_size_gx2 __attribute__((section(".data"))) = sizeof(method_hooks_gx2) / sizeof(FunctionHook);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_gx2[sizeof(method_hooks_gx2) / sizeof(FunctionHook) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));