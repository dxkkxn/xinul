/*
 * Projet PCSEA RISC-V
 *
 * Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
 * Mathieu Barbe <mathieu@kolabnow.com> - 2019
 *
 * See license for license details.
 */

#include "stdlib.h"
#include "assert.h"

#include "encoding.h"
#include "csr.h"
#include "info.h"
#include "trap.h"

// Prototypes externes
// kernel/start.c
extern int kernel_start();
// kernel/boot/$(MACHINE)/setup.c
extern void arch_setup();
// kernel/trap/supervisor_trap_entry.S
extern void strap_entry();

// send S-mode interrupts and most exceptions straight to S-mode
static void delegate_traps()
{
	uintptr_t interrupts = MIP_SSIP | MIP_STIP | MIP_SEIP;
	uintptr_t exceptions =
			(1U << CAUSE_MISALIGNED_FETCH) |
			(1U << CAUSE_FETCH_PAGE_FAULT) |
			(1U << CAUSE_BREAKPOINT) |
			(1U << CAUSE_LOAD_PAGE_FAULT) |
			(1U << CAUSE_STORE_PAGE_FAULT) |
			(1U << CAUSE_USER_ECALL);

	csr_write(mideleg, interrupts);
	csr_write(medeleg, exceptions);
	assert(csr_read(mideleg) == interrupts);
//	assert(csr_read(medeleg) == exceptions);
// Commenté car pour la zybo, on ne peut délèguer les exceptions de mémoire virtuelle (pas de VM)
}

static inline void setup_pmp(void)
{
	// Set up a PMP to permit access to all of memory.
	// Ignore the illegal-instruction trap if PMPs aren't supported.
	uintptr_t pmpc = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
	uintptr_t pmpa = ((uintptr_t) 1 << (__riscv_xlen == 32 ? 31 : 53)) - 1;
	__asm__ __volatile__ ("la t0, 1f\n\t"
						  "csrrw t0, mtvec, t0\n\t"
						  "csrw pmpaddr0, %1\n\t"
						  "csrw pmpcfg0, %0\n\t"
						  ".align 2\n\t"
						  "1: csrw mtvec, t0"
	: : "r" (pmpc), "r" (pmpa) : "t0");
}

static inline void enter_supervisor_mode()
{

	setup_pmp();

	// bug hardware zybo ici :
	// si on configure mstatus/mpp avant mie,
	// csrw mie modifi mstatus mpp à 0
	// on neva en mode user plutôt quand mode supervisor
	// set the previous context in mstatus
	csr_set(mstatus, MSTATUS_MPP & MSTATUS_MPP_S);

	__asm__ __volatile__ (
	"la t0, 1f\n"
	"csrw mepc, t0\n"
	"mret\n"
	"1:":: : "t0"
	);
}


// this function is called by entry.S
// Only the interrupt vector is set up and we are still in machine mode
__attribute__((noreturn)) void boot_riscv()
{
	// Configuration spécifique à la machine utilisé, (uart / htif).
	arch_setup();

	display_info_proc();

	// On active les intéruption software et timer
	// todo a comprendre : Si m software non actif, on peut quand même utiliser mcall
	csr_write(mie, MIP_MSIP | MIP_MTIP);

	// Désactivation temporaire de la mémoire virtuelle (bare memory)
	csr_write(satp, 0);

	delegate_traps();

	// Configuration du plic
// todo comprendre à quel mement il est configuré

	// Configuration de supervisor trap vector (direct mode)
	csr_write(stvec, (unsigned long) strap_entry | 0UL);

	// Mise à zéro des registres s
	csr_write(sscratch, 0);
	csr_write(sie, 0);

	enter_supervisor_mode();
	exit(kernel_start());
	__builtin_unreachable();
}
