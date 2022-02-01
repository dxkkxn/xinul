# Benoît Wallon <benoit.wallon@grenoble-inp.org> - 2019
# Mathieu Barbe <mathieu@kolabnow.com> - 2019
# Frédéric Pétrot <frederic.petrot@univ-grenoble-alpes.fr> - 2022
# See license for license details.

# Le boot le plus simple du monde

.section .text.init,"ax",@progbits
.globl _start
_start:
    la      sp, mstack + 2040
    j       main

# Réservons une pile de 1024 mots de 64 bits
mstack:
    .skip 4096
