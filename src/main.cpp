#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "main.h"
#include "common/common.h"

#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "patcher/coreinit_function_patcher.h"
#include "patcher/fs_function_patcher.h"
#include "patcher/pad_function_patcher.h"
#include "kernel/kernel_functions.h"
#include "utils/logger.h"
#include "patcher/gx2_function_patcher.h"

/* Entry point */
extern "C" int Menu_Main(void) {
	//!*******************************************************************
	//!                   Initialize function pointers                   *
	//!*******************************************************************
	//! Acquire every RPL we want to patch

	InitOSFunctionPointers();
	InitSocketFunctionPointers(); //For logging

	InitSysFunctionPointers(); // For SYSLaunchMenu()

	//For patching
	InitVPadFunctionPointers();
	InitPadScoreFunctionPointers();
	InitGX2FunctionPointers();

	SetupKernelCallback();

	log_init("192.168.2.103");

	//Reset everything when were going back to the Mii Maker
	if (strlen(cosAppXmlInfoStruct.rpx_name) > 0 && strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) == 0) {
		log_print("Returing to the Homebrew Launcher!\n");
		restoreFunctionPatches();
		return EXIT_SUCCESS;
	}

	//!*******************************************************************
	//!                        Patching functions                        *
	//!*******************************************************************
	log_print("Patching functions\n");
	applyFunctionPatches();

	if (strlen(cosAppXmlInfoStruct.rpx_name) > 0 &&
		strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) != 0) //Starting the application
	{
		return EXIT_RELAUNCH_ON_LOAD;
	}

	if (strlen(cosAppXmlInfoStruct.rpx_name) <= 0) { // First boot back to SysMenu
		SYSLaunchMenu();
		return EXIT_RELAUNCH_ON_LOAD;
	}

	restoreFunctionPatches();
	return EXIT_SUCCESS;
}

void applyFunctionPatches() {
	patchIndividualMethodHooks(method_hooks_coreinit, method_hooks_size_coreinit, method_calls_coreinit);
	patchIndividualMethodHooks(method_hooks_fs, method_hooks_size_fs, method_calls_fs);
	patchIndividualMethodHooks(method_hooks_pad, method_hooks_size_pad, method_calls_pad);
	patchIndividualMethodHooks(method_hooks_gx2, method_hooks_size_gx2, method_calls_gx2);
}

void restoreFunctionPatches() {
	restoreIndividualInstructions(method_hooks_coreinit, method_hooks_size_coreinit);
	restoreIndividualInstructions(method_hooks_fs, method_hooks_size_fs);
	restoreIndividualInstructions(method_hooks_pad, method_hooks_size_pad);
	log_deinit();
}