#ifndef _KERNEL_SBI_H_
#define _KERNEL_SBI_H_

#include "stdint.h"

void handle_mtimer_interrupt();
uint64_t handle_sbi_call(uint64_t call_no, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2);

#define SBI_CALL(call_no, arg0, arg1, arg2) ({					\
	register uintptr_t a0 __asm__ ("a0") = (uintptr_t)(arg0);	\
	register uintptr_t a1 __asm__ ("a1") = (uintptr_t)(arg1);	\
	register uintptr_t a2 __asm__ ("a2") = (uintptr_t)(arg2);	\
	register uintptr_t a7 __asm__ ("a7") = (uintptr_t)(call_no);	\
	__asm__ __volatile__ ("ecall"								\
			: "+r" (a0)											\
			: "r" (a1), "r" (a2), "r" (a7)						\
			: "memory");										\
	a0;															\
})

#define SBI_CALL_0(call_no) SBI_CALL(call_no, 0, 0, 0)
#define SBI_CALL_1(call_no, arg0) SBI_CALL(call_no, arg0, 0, 0)
#define SBI_CALL_2(call_no, arg0, arg1) SBI_CALL(call_no, arg0, arg1, 0)
#define SBI_CALL_3(call_no, arg0, arg1, arg2) \
	SBI_CALL(call_no, arg0, arg1, arg2)

// value is given in ms
void sbi_call_set_timer(uint64_t value);

#endif /* _KERNEL_SBI_H_ */
