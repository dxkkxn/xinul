
#ifndef SRC_TRAP_H
#define SRC_TRAP_H

enum exception_cause_flag {
	CAUSE_MISALIGNED_FETCH = 0x0,
	CAUSE_FETCH_ACCESS = 0x1,
	CAUSE_ILLEGAL_INSTRUCTION = 0x2,
	CAUSE_BREAKPOINT = 0x3,
	CAUSE_MISALIGNED_LOAD = 0x4,
	CAUSE_LOAD_ACCESS = 0x5,
	CAUSE_MISALIGNED_STORE = 0x6,
	CAUSE_STORE_ACCESS = 0x7,
	CAUSE_USER_ECALL = 0x8,
	CAUSE_SUPERVISOR_ECALL = 0x9,
	CAUSE_HYPERVISOR_ECALL = 0xa,
	CAUSE_MACHINE_ECALL = 0xb,
	CAUSE_FETCH_PAGE_FAULT = 0xc,
	CAUSE_LOAD_PAGE_FAULT = 0xd,
	CAUSE_STORE_PAGE_FAULT = 0xf,
};

enum interruption_cause_flag {
	intr_u_software = 0,
	intr_s_software = 1,
	intr_h_software = 2,
	intr_m_software = 3,
	intr_u_timer = 4,
	intr_s_timer = 5,
	intr_h_timer = 6,
	intr_m_timer = 7,
	intr_u_external = 8,
	intr_s_external = 9,
	intr_h_external = 10,
	intr_m_external = 11,
};


struct trap_frame {
	uint64_t ra;
	uint64_t a0;
	uint64_t a1;
	uint64_t a2;
	uint64_t a3;
	uint64_t a4;
	uint64_t a5;
	uint64_t a6;
	uint64_t a7;
	uint64_t t0;
	uint64_t t1;
	uint64_t t2;
	uint64_t t3;
	uint64_t t4;
	uint64_t t5;
	uint64_t t6;
	uint64_t s0;
	uint64_t s1;
	uint64_t s2;
	uint64_t s3;
	uint64_t s4;
	uint64_t s5;
	uint64_t s6;
	uint64_t s7;
	uint64_t s8;
	uint64_t s9;
	uint64_t s10;
	uint64_t s11;
	uint64_t tp;
	uint64_t gp;
	uint64_t sp;

	// csr
	uint64_t sstatus;
	uint64_t sepc;
	uint64_t stval;
	uint64_t scause;
	uint32_t insn;
};


#endif //SRC_TRAP_H
