#include "stdio.h"
#include "stddef.h"

#include "syscall_handler.h"


void* syscall_handler(uint64_t call_no, void* arg0, void* arg1, void* arg2, void* arg4, void* arg5)
{
	printf("syscall handler no %ld start :\narg0 %p, arg1 %p, arg2 %p\n", call_no, arg0, arg1, arg2);
	
	
	return (void*)0;
}
