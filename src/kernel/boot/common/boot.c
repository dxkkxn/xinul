// See LICENSE for license details.

#include "encoding.h"
#include "machine.h"
#include "info.h"
#include "trap.h"

// todo a virer si pas de bug
//#define MSTATUS_MPP_MASK_S 0x800 /* bit 12-11 = 01 */
//#define MSTATUS_MPIE_MASK 0x80 /* bit 7 = 1 */

extern void strap_entry();


int main();


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

	write_csr(mideleg, interrupts);
	write_csr(medeleg, exceptions);
	assert(read_csr(mideleg) == interrupts);
	assert(read_csr(medeleg) == exceptions);
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

	// todo : bug ici :
	// si on configure mstatus/mpp avant mie,
	// csrw mie modifi mstatus mpp à 0
	// on neva en mode user plutôt quand mode supervisor
	// set the previous context in mstatus
	set_csr(mstatus, MSTATUS_MPP & MSTATUS_MPP_S);

	__asm__ __volatile__ (
	"la t0, 1f\n"
	"csrw mepc, t0\n"
	"mret\n"
	"1:":: : "t0"
	);
}

// todo à virer
#include "vmm.h"
#include "hmm.h"

// this function is called by entry.S
// Only the interrupt vector is set up and we are still in machine mode
__attribute__((noreturn)) void boot_riscv()
{
	// Configuration spécifique à la machine utilisé, (uart / htif).
	arch_setup();

	display_info_proc();

	// On active les intéruption software et timer
	// todo a comprendre : Si m software non actif, on peut quand même utiliser mcall
	write_csr(mie, MIP_MSIP | MIP_MTIP);

	// Désactivation temporaire de la mémoire virtuelle (bare memory)
	write_csr(satp, 0);

	delegate_traps();

	// Configuration du plic
// todo comprendre à quel mement il est configuré

	// Configuration de supervisor trap vector (direct mode)
	write_csr(stvec, (unsigned long) strap_entry | 0UL);


	// Mise à zéro des registres s
	write_csr(sscratch, 0);
	write_csr(sie, 0);

	// todo a vriervirer mais garder pour test harware
	set_csr(sstatus, SSTATUS_SUM | SSTATUS_FS);
	printf("Hardware memory manager initialization...");
	extern char _free_memory_start[];
	extern char _memory_end[];
	extern char _bss_start[];
	extern char _bss_end[];
	(void) _bss_end;
	(void) _bss_start;
	hmm_init(_free_memory_start, _memory_end);
	printf("\r\t\t\t\t\t\t\t[OK]\n");
	printf("Number of frames %d = %d ko\n", hmm_frame_count(), hmm_free_memory() >> 10);

	init_virtual_memory();

	enter_supervisor_mode();
	exit(main());
	__builtin_unreachable();
}
