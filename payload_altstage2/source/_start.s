.section .text.start
.align 4
.global _start
_start:

    bl main

.die:
    b .die
