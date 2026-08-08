.extern _start
.global _reset_vector
.code 32

.section .reset , "ax"

_reset_vector: 
    B _start
