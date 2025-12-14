### Subroutines called from C.
    
### You should be able to compile this with each of the
### test programs to try out each of the functions.
    
    ## Expose the names of all of our subroutines.
    .global setGlobal
    .global swapGlobals
    .global sixParams
    .global formula

    ## Code goes in the text section.
    .text

### Implementation of setGlobal, assign 47 to a global variable.
setGlobal:
    mov $47, %eax
    mov %eax, globalVar

    ret                         # Return to the caller.
    
### Implementation of swapGlobals, exchange the value of two global variables
swapGlobals:
    mov globalA, %eax
    mov globalB, %ebx
    mov %eax, globalB
    mov %ebx, globalA

    ret

sixParams:
    mov %edi, %eax
    imul %ecx, %eax
    mov %esi, %ebx
    imul %r8d, %ebx
    mov %edx, %ecx
    imul %r9d, %ecx
    add %ebx, %eax
    sub %ecx, %eax

    ret

formula:
    mov globalA, %eax
    imul %esi, %eax
    mov $100, %ebx
    sub %eax, %ebx
    imul %edi, %ebx
    mov globalB, %ecx
    imul %ecx, %ecx
    mov $3, %edx
    imul %edx, %ecx
    add %ebx, %ecx
    mov %ecx, %eax

    ret
