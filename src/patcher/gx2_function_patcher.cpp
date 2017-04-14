/****************************************************************************
 * Copyright (C) 2017 BullyWiiPlaza
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

#include "pad_function_patcher.h"
#include "../utils/logger.h"

static volatile int executionCounter = 0;

DECL(void, GX2CopyColorBufferToScanBuffer, const GX2ColorBuffer *colorBuffer, s32 scan_target) {
	if (executionCounter > 120) {
		GX2Surface surface = colorBuffer->surface;
		log_printf("GX2CopyColorBufferToScanBuffer {surface width:%d, height:%d, image size:%d, image data:%x}\n", surface.width, surface.height, surface.image_size, surface.image_data);

		executionCounter = 0;
	}

	executionCounter++;
	real_GX2CopyColorBufferToScanBuffer(colorBuffer, scan_target);
}

hooks_magic_t method_hooks_gx2[] __attribute__((section(".data"))) = {
		MAKE_MAGIC(GX2CopyColorBufferToScanBuffer, LIB_GX2, STATIC_FUNCTION)
};

u32 method_hooks_size_gx2 __attribute__((section(".data"))) = sizeof(method_hooks_gx2) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_gx2[sizeof(method_hooks_gx2) / sizeof(hooks_magic_t) *
									   FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));