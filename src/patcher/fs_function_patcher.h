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

#ifndef _FS_FUNCTION_PATCHER_H
#define _FS_FUNCTION_PATCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../utils/function_patcher.h"

extern FunctionHook method_hooks_fs[];
extern u32 method_hooks_size_fs;
extern volatile unsigned int method_calls_fs[];

#ifdef __cplusplus
}
#endif

#endif /* _FS_FUNCTION_PATCHER_H */