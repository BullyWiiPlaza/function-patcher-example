/****************************************************************************
 * Copyright (C) 2016 Maschell
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

#include "coreinit_function_patcher.h"

#include "../utils/logger.h"

declareFunctionHook(void, _Exit, void) {
	log_print("Application closed\n");
	real__Exit();
}

FunctionHook method_hooks_coreinit[] __attribute__((section(".data"))) = {
		makeFunctionHook(_Exit, LIB_CORE_INIT, STATIC_FUNCTION),
};

u32 method_hooks_size_coreinit __attribute__((section(".data"))) = sizeof(method_hooks_coreinit) / sizeof(FunctionHook);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_coreinit[sizeof(method_hooks_coreinit) / sizeof(FunctionHook) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));