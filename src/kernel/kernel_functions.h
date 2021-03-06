#ifndef __KERNEL_FUNCTIONS_H_
#define __KERNEL_FUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "syscalls.h"
#include "../common/kernel_defs.h"

void SetupKernelCallback(void);

extern ReducedCosAppXmlInfo cosAppXmlInfoStruct;

#ifdef __cplusplus
}
#endif

#endif // __KERNEL_FUNCTIONS_H_
