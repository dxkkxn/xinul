#  traitant d'interruption
# takes handler adress as parameter

.text 

.globl traitant_IT

traitant_IT:
    # save context
    
    # this has to be atomic => SIE is set to 0
    csrw mscratch, mstatus
    andi mscratch, mscratch, ~0x00000002
    csrw mstatus, mscratch

    # save sstatus
    
    # save pc into sepc
    add sepc, pc, zero
    # pc pointe vers le traitant
    add pc, a0, zero

    ret