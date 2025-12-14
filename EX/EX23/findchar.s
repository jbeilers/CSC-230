### Subroutine to find a given character in a string and return
### its index.
### Fill in the rest of this file.

.global findChar

findChar:
    lea (%rdi), %ebx
    call strlen
    mov $0, %ecx
compare:
    cmp %eax, %ecx
    jge nofound
    lea (%ebx), %rdx
    cmp %rdx, %rsi
    je found
    inc %ecx
    inc %ebx
    jmp compare
found:
    mov %ecx, %eax
    ret
nofound:
    mov $-1, %eax
    ret
