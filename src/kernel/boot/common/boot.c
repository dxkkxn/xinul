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
#include "riscv.h"

#include "bios/info.h"
#include "traps/trap.h"

/*
 * Prototypes externes
 */
// kernel/start.c
extern int kernel_start();

// Specific machine setup: kernel/boot/$(MACHINE)/setup.c
extern void arch_setup();

#ifndef STUDENT
// kernel/trap/supervisor_trap_entry.S
extern void strap_entry();
#endif // STUDENT

static void delegate_traps()
{
#ifndef STUDENT
	// send S-mode interrupts and most exceptions straight to S-mode
	uintptr_t interrupts = MIP_SSIP | MIP_STIP | MIP_SEIP;
	uintptr_t exceptions = (1U << CAUSE_USER_ECALL);
//			(1U << CAUSE_MISALIGNED_FETCH) |
//			(1U << CAUSE_FETCH_PAGE_FAULT) |
//			(1U << CAUSE_BREAKPOINT) |
//			(1U << CAUSE_LOAD_PAGE_FAULT) |
//			(1U << CAUSE_STORE_PAGE_FAULT) |

	csr_write(mideleg, interrupts);
	csr_write(medeleg, exceptions);
#else // STUDENT

	/*
	 * Délégations de certaines interruptions et exceptions vers le mode Supervisor.
	 * A compléter au fur et à mesure du projet celon les besoins.
	 * CSR concernés: mideleg et medeleg.
	 */
#endif // STUDENT
}

static inline void setup_pmp(void)
{
	/*
	 * Désactivation de la protection de la mémoire physique (PMP).
	 *
	 * Configuration de la PMP pour donner  un accès complet à la mémoire.
	 * CSR concernés: pmpaddr0 and pmpcfg0.
	 */

#ifndef STUDENT
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
#endif // STUDENT
}

static inline void enter_supervisor_mode()
{

	// Il faut obligatoirement configurer la protection de la mémoire physique avant de passer en mode supervisor.
	setup_pmp();

	/*
	 * Configuration du mode à utiliser lors de l'instruction mret.
	 *
	 * CSR concernés: mepc et mstatus.
	 * Voir aussi riscv.h pour la macro mret().
	 */

#ifndef STUDENT
	/* bug hardware zybo ici :
	 * si on configure le champ mstatus/mpp avant le csr mie,
	 * csrw mie modifie le champ mstatus mpp à 0
	 * conséquence : on va en mode user plutôt qu'en mode supervisor
	 */
	csr_set(mstatus, MSTATUS_MPP & MSTATUS_MPP_S);

	__asm__ __volatile__ (
	"la t0, 1f\n"
	"csrw mepc, t0\n"
	"mret\n"
	"1:":: : "t0"
	);

#endif // STUDENT
}

/*
 * boot_riscv
 *
 * Cette fonction est appelée depuis crtm.S
 * A ce stade, seul le vecteur de trap machine mtvec a été configuré.
 * La pile en cours d'utilisation est la pile machine qui a été alouée dans crtm.S.
 * Cette pile est également utilisée pour traiter les traps machine.
 *
 * Le processeur est toujours en mode machine
 */
__attribute__((noreturn)) void boot_riscv()
{
	// Configuration des composants spécifiques à la machine (uart / htif, timer et interruptions externes).
	arch_setup();

	display_info_proc();

	// Délégations des interruptions et des exceptions
	delegate_traps();
#ifndef STUDENT

	// Désactivation temporaire de la mémoire virtuelle (bare memory) (normalement déjà désactivée)
	csr_write(satp, 0);

	// Configuration de supervisor trap vector (direct mode bit 0 à 0)
	csr_write(stvec, (unsigned long) strap_entry);

	// Mise à zéro des registres supervisor importants (pas forcement nécessaire)
	csr_write(sscratch, 0);
	csr_write(sie, 0);

#endif // STUDENT

	enter_supervisor_mode();
	exit(kernel_start());
	__builtin_unreachable();
}
