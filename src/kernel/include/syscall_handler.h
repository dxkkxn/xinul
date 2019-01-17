#ifndef SYSCALL_HANDLER_H
#define SYSCALL_HANDLER_H

#include "stdint.h"

void* syscall_handler(uint64_t call_no, void* arg0, void* arg1, void* arg2, void* arg3, void* arg4);

#endif
